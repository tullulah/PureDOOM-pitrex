#!/usr/bin/env python3
"""
vec_to_c.py  — convert Vectrex Studio .vec file(s) to a C header.

Single-file mode (original):
    python3 tools/vec_to_c.py assets/doom_logo.vec src/doom_logo.h

Multi-file mode (combined header):
    python3 tools/vec_to_c.py assets/hud/key_card.vec assets/hud/key_skull.vec -o src/hud_assets.h

Output:
  - Flat 2D segment arrays {x0,y0,x1,y1}  → for draw_vec_shape()
  - 3D vertex-indexed tables               → for draw_vec_shape_3d()
    Format mirrors the VPY compiler (vecres.rs compile_to_3d_asm_with_name):
      vertex_count (int)
      {x,y,z} × vertex_count  (coords clamped ±63)
      path_count (int)
      per path: pt_count, closed, idx0, idx1, ...
"""

import json
import sys
import os
from datetime import date


def extract_segments(vec_path: str):
    """Return (c_name, segments_list) from a .vec file."""
    with open(vec_path, "r") as f:
        data = json.load(f)

    name = data.get("name", os.path.splitext(os.path.basename(vec_path))[0])
    c_name = name.lower().replace("-", "_").replace(" ", "_")

    segments = []  # list of (x0, y0, x1, y1, path_name)
    for layer in data.get("layers", []):
        if not layer.get("visible", True):
            continue
        for path in layer.get("paths", []):
            pts = path.get("points", [])
            pname = path.get("name", "?")
            closed = path.get("closed", False)
            if len(pts) < 2:
                continue
            for i in range(len(pts) - 1):
                segments.append((
                    int(pts[i]["x"]),   int(pts[i]["y"]),
                    int(pts[i+1]["x"]), int(pts[i+1]["y"]),
                    pname
                ))
            if closed:
                segments.append((
                    int(pts[-1]["x"]),  int(pts[-1]["y"]),
                    int(pts[0]["x"]),   int(pts[0]["y"]),
                    pname
                ))
    return c_name, segments


def clamp63(v):
    return max(-63, min(63, int(v)))


def extract_3d(vec_path: str):
    """Return (c_name, vertices, paths) for 3D vertex-indexed format.
    vertices: list of (x,y,z) clamped ±63
    paths: list of {'closed': bool, 'indices': [int,...]}
    Matches VPY compiler format (vecres.rs compile_to_3d_asm_with_name).
    """
    with open(vec_path, "r") as f:
        data = json.load(f)

    name = data.get("name", os.path.splitext(os.path.basename(vec_path))[0])
    c_name = name.lower().replace("-", "_").replace(" ", "_")

    vertex_map = {}  # (x,y,z) -> index
    vertices = []
    paths = []

    def get_vertex(x, y, z):
        key = (x, y, z)
        if key not in vertex_map:
            vertex_map[key] = len(vertices)
            vertices.append(key)
        return vertex_map[key]

    for layer in data.get("layers", []):
        if not layer.get("visible", True):
            continue
        for path in layer.get("paths", []):
            pts = path.get("points", [])
            if len(pts) < 2:
                continue
            closed = path.get("closed", False)
            indices = []
            for pt in pts:
                x = clamp63(pt.get("x", 0))
                y = clamp63(pt.get("y", 0))
                z = clamp63(pt.get("z", 0))
                indices.append(get_vertex(x, y, z))
            paths.append({"closed": closed, "indices": indices})

    return c_name, vertices, paths


def emit_array(lines, c_name, segments, vec_path):
    lines.append(f"/* --- {os.path.basename(vec_path)} --- */")
    lines.append(f"static const vec_seg_t {c_name}_segs[] = {{")
    current_path = None
    for x0, y0, x1, y1, pname in segments:
        if pname != current_path:
            lines.append(f"    /* {pname} */")
            current_path = pname
        lines.append(f"    {{ {x0:4d}, {y0:4d}, {x1:4d}, {y1:4d} }},")
    lines.append("};")
    lines.append(f"static const int {c_name}_seg_count =")
    lines.append(f"    (int)(sizeof({c_name}_segs) / sizeof({c_name}_segs[0]));")
    lines.append("")


def emit_3d_table(lines, c_name, vertices, paths, vec_path):
    """Emit vertex-indexed 3D table matching VPY compiler format."""
    total_pt_refs = sum(len(p["indices"]) for p in paths)
    lines.append(f"/* --- {os.path.basename(vec_path)} 3D table ---")
    lines.append(f" * {len(vertices)} unique verts, {len(paths)} paths, {total_pt_refs} total point refs */")
    # Vertex struct
    lines.append(f"static const vec3d_vert_t {c_name}_verts[] = {{")
    for x, y, z in vertices:
        lines.append(f"    {{ {x:4d}, {y:4d}, {z:4d} }},")
    lines.append("};")
    lines.append(f"static const int {c_name}_vert_count = {len(vertices)};")
    lines.append("")
    # Path index arrays
    for pi, path in enumerate(paths):
        idx_list = ", ".join(str(i) for i in path["indices"])
        closed = 1 if path["closed"] else 0
        lines.append(f"static const uint8_t {c_name}_path{pi}_idx[] = {{ {idx_list} }};")
    lines.append(f"static const vec3d_path_t {c_name}_paths[] = {{")
    for pi, path in enumerate(paths):
        closed = 1 if path["closed"] else 0
        lines.append(f"    {{ {len(path['indices'])}, {closed}, {c_name}_path{pi}_idx }},")
    lines.append("};")
    lines.append(f"static const int {c_name}_path_count = {len(paths)};")
    lines.append("")


def emit_3d_typedefs(lines):
    lines.append("#ifndef VEC3D_TYPES_DEFINED")
    lines.append("#define VEC3D_TYPES_DEFINED")
    lines.append("typedef struct { int8_t x, y, z; } vec3d_vert_t;")
    lines.append("typedef struct { int pt_count; int closed; const uint8_t* idx; } vec3d_path_t;")
    lines.append("#endif")
    lines.append("")


def convert_single(vec_path: str, out_path: str) -> None:
    c_name, segments = extract_segments(vec_path)
    _, vertices, paths = extract_3d(vec_path)
    guard = c_name.upper() + "_H"

    lines = []
    lines.append(f"/* Auto-generated from {os.path.basename(vec_path)}")
    lines.append(f" * by tools/vec_to_c.py — {date.today()} — DO NOT EDIT */")
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}")
    lines.append("")
    lines.append("#include <stdint.h>")
    lines.append("#ifndef VEC_SEG_T_DEFINED")
    lines.append("#define VEC_SEG_T_DEFINED")
    lines.append("typedef struct { int16_t x0, y0, x1, y1; } vec_seg_t;")
    lines.append("#endif")
    lines.append("")
    emit_3d_typedefs(lines)
    emit_array(lines, c_name, segments, vec_path)
    emit_3d_table(lines, c_name, vertices, paths, vec_path)
    lines.append(f"#endif /* {guard} */")

    with open(out_path, "w") as f:
        f.write("\n".join(lines) + "\n")
    print(f"Wrote {len(segments)} segments, {len(vertices)} verts → {out_path}")


def convert_multi(vec_paths: list, out_path: str) -> None:
    guard = os.path.splitext(os.path.basename(out_path))[0].upper() + "_H"

    lines = []
    lines.append(f"/* Auto-generated from {', '.join(os.path.basename(p) for p in vec_paths)}")
    lines.append(f" * by tools/vec_to_c.py — {date.today()} — DO NOT EDIT */")
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}")
    lines.append("")
    lines.append("#include <stdint.h>")
    lines.append("#ifndef VEC_SEG_T_DEFINED")
    lines.append("#define VEC_SEG_T_DEFINED")
    lines.append("typedef struct { int16_t x0, y0, x1, y1; } vec_seg_t;")
    lines.append("#endif")
    lines.append("")
    emit_3d_typedefs(lines)

    total_segs = 0
    for vp in vec_paths:
        c_name, segments = extract_segments(vp)
        _, vertices, paths = extract_3d(vp)
        emit_array(lines, c_name, segments, vp)
        emit_3d_table(lines, c_name, vertices, paths, vp)
        total_segs += len(segments)
        print(f"  {len(segments):3d} segs  {len(vertices):3d} verts  {os.path.basename(vp)}")

    lines.append(f"#endif /* {guard} */")
    with open(out_path, "w") as f:
        f.write("\n".join(lines) + "\n")
    print(f"Wrote {total_segs} total segments → {out_path}")


if __name__ == "__main__":
    args = sys.argv[1:]
    if "-o" in args:
        idx = args.index("-o")
        out_path = args[idx + 1]
        vec_paths = [a for a in args if a != "-o" and a != out_path]
        if not vec_paths:
            print("Error: no input .vec files", file=sys.stderr)
            sys.exit(1)
        convert_multi(vec_paths, out_path)
    elif len(args) == 2:
        convert_single(args[0], args[1])
    else:
        print(f"Usage:", file=sys.stderr)
        print(f"  {sys.argv[0]} input.vec output.h", file=sys.stderr)
        print(f"  {sys.argv[0]} a.vec b.vec ... -o output.h", file=sys.stderr)
        sys.exit(1)
