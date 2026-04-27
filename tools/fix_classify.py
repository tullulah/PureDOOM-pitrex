import sys

with open('src/renderer.c', 'r', encoding='utf-8') as f:
    src = f.read()

# 1. Add classify_sprite() before renderer_draw_sprites
classify_fn = """/* classify_sprite: 0=enemy, 1=pickup, -1=skip (blood/puff/projectile/player) */
static int classify_sprite(int patch_idx)
{
    const char* n = lumpinfo[firstspritelump + patch_idx].name;
    /* Skip: blood splats, bullet puffs, player corpse, projectiles, smoke */
    if (n[0]=='B'&&n[1]=='L'&&n[2]=='U'&&n[3]=='D') return -1;
    if (n[0]=='P'&&n[1]=='U'&&n[2]=='F'&&n[3]=='F') return -1;
    if (n[0]=='P'&&n[1]=='L'&&n[2]=='A'&&n[3]=='Y') return -1;
    if (n[0]=='B'&&n[1]=='A'&&n[2]=='L') return -1; /* BAL1/BAL2 fireballs */
    if (n[0]=='M'&&n[1]=='I'&&n[2]=='S'&&n[3]=='L') return -1; /* rocket in flight */
    if (n[0]=='M'&&n[1]=='A'&&n[2]=='N'&&n[3]=='F') return -1; /* plasma */
    if (n[0]=='A'&&n[1]=='P'&&n[2]=='L') return -1; /* APLS/APLX plasma ball */
    if (n[0]=='R'&&n[1]=='B'&&n[2]=='A'&&n[3]=='L') return -1; /* rev tracer */
    if (n[0]=='T'&&n[1]=='R'&&n[2]=='A'&&n[3]=='C') return -1; /* tracer */
    /* Enemies (by 4-char sprite prefix) */
    if (n[0]=='P'&&n[1]=='O'&&n[2]=='S'&&n[3]=='S') return 0;
    if (n[0]=='S'&&n[1]=='P'&&n[2]=='O'&&n[3]=='S') return 0;
    if (n[0]=='C'&&n[1]=='P'&&n[2]=='O'&&n[3]=='S') return 0;
    if (n[0]=='V'&&n[1]=='I'&&n[2]=='L'&&n[3]=='E') return 0;
    if (n[0]=='T'&&n[1]=='R'&&n[2]=='O'&&n[3]=='O') return 0;
    if (n[0]=='S'&&n[1]=='A'&&n[2]=='R'&&n[3]=='G') return 0;
    if (n[0]=='H'&&n[1]=='E'&&n[2]=='A'&&n[3]=='D') return 0;
    if (n[0]=='S'&&n[1]=='K'&&n[2]=='U'&&n[3]=='L') return 0;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='S'&&n[3]=='S') return 0;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='S'&&n[3]=='2') return 0;
    if (n[0]=='B'&&n[1]=='S'&&n[2]=='P'&&n[3]=='I') return 0;
    if (n[0]=='C'&&n[1]=='Y'&&n[2]=='B'&&n[3]=='R') return 0;
    if (n[0]=='S'&&n[1]=='P'&&n[2]=='I'&&n[3]=='D') return 0;
    if (n[0]=='P'&&n[1]=='A'&&n[2]=='I'&&n[3]=='N') return 0;
    if (n[0]=='S'&&n[1]=='K'&&n[2]=='E'&&n[3]=='L') return 0;
    if (n[0]=='F'&&n[1]=='A'&&n[2]=='T'&&n[3]=='T') return 0;
    /* Everything else: treat as pickup / decoration */
    return 1;
}

"""

old_fn_header = 'void renderer_draw_sprites(void)'
if old_fn_header not in src:
    print("ERROR: renderer_draw_sprites not found"); sys.exit(1)
src = src.replace(old_fn_header, classify_fn + old_fn_header, 1)

# 2. Replace the ph_raw-based classification with classify_sprite
old_block = (
    '        if      (ph_raw >= 40) {\n'
    '            int et = get_enemy_type(vis->patch);\n'
    '            if (et == ET_OTHER) {\n'
    '                static char seen[32][9]; static int nseen = 0;\n'
    '                const char* nm = lumpinfo[firstspritelump + vis->patch].name;\n'
    '                int found = 0;\n'
    '                for (int k = 0; k < nseen; k++) if (doom_strncmp(seen[k], nm, 8) == 0) { found = 1; break; }\n'
    '                if (!found && nseen < 32) {\n'
    '                    doom_strncpy(seen[nseen++], nm, 8);\n'
    '                    printf("[UNKNOWN ENEMY]  lump=%.8s  ph=%d\\n", nm, ph_raw);\n'
    '                }\n'
    '            }\n'
    '            draw_enemy(cx, cy, r, et);\n'
    '        } else {\n'
    '            int pt = get_pickup_type(vis->patch);\n'
    '            if (pt == PT_OTHER) {\n'
    '                static char seen[32][9]; static int nseen = 0;\n'
    '                const char* nm = lumpinfo[firstspritelump + vis->patch].name;\n'
    '                int found = 0;\n'
    '                for (int k = 0; k < nseen; k++) if (doom_strncmp(seen[k], nm, 8) == 0) { found = 1; break; }\n'
    '                if (!found && nseen < 32) {\n'
    '                    doom_strncpy(seen[nseen++], nm, 8);\n'
    '                    printf("[UNKNOWN PICKUP] lump=%.8s  ph=%d\\n", nm, ph_raw);\n'
    '                }\n'
    '            }\n'
    '            draw_pickup(cx, cy, r, pt);\n'
    '        }'
)

new_block = (
    '        int cls = classify_sprite(vis->patch);\n'
    '        if (cls < 0) continue; /* blood / puff / player / projectile */\n'
    '        if (cls == 0) {\n'
    '            int et = get_enemy_type(vis->patch);\n'
    '            if (et == ET_OTHER) {\n'
    '                static char seen[32][9]; static int nseen = 0;\n'
    '                const char* nm = lumpinfo[firstspritelump + vis->patch].name;\n'
    '                int found = 0;\n'
    '                for (int k = 0; k < nseen; k++) if (doom_strncmp(seen[k], nm, 8) == 0) { found = 1; break; }\n'
    '                if (!found && nseen < 32) {\n'
    '                    doom_strncpy(seen[nseen++], nm, 8);\n'
    '                    printf("[UNKNOWN ENEMY]  lump=%.8s  ph=%d\\n", nm, ph_raw);\n'
    '                }\n'
    '            }\n'
    '            draw_enemy(cx, cy, r, et);\n'
    '        } else {\n'
    '            int pt = get_pickup_type(vis->patch);\n'
    '            if (pt == PT_OTHER) {\n'
    '                static char seen[32][9]; static int nseen = 0;\n'
    '                const char* nm = lumpinfo[firstspritelump + vis->patch].name;\n'
    '                int found = 0;\n'
    '                for (int k = 0; k < nseen; k++) if (doom_strncmp(seen[k], nm, 8) == 0) { found = 1; break; }\n'
    '                if (!found && nseen < 32) {\n'
    '                    doom_strncpy(seen[nseen++], nm, 8);\n'
    '                    printf("[UNKNOWN PICKUP] lump=%.8s  ph=%d\\n", nm, ph_raw);\n'
    '                }\n'
    '            }\n'
    '            draw_pickup(cx, cy, r, pt);\n'
    '        }'
)

if old_block in src:
    src = src.replace(old_block, new_block, 1)
    print("OK: replaced classification block")
else:
    print("ERROR: old block not found")
    sys.exit(1)

with open('src/renderer.c', 'w', encoding='utf-8') as f:
    f.write(src)
print("Done")
