#!/usr/bin/env python3
"""led-1d-coordinates.csv の任意の 0/1 列から、config.h 用のビットマスクを生成する。

usage: python3 tools/gen-zone-mask.py [列名]   (既定: 3x5領域か)

出力される #define をそのまま config.h に貼る。チェーンインデックス gi に対する
判定は rgblight_call_driver 側の RGB_IS_3X5(gi) が行う。
"""
import csv, pathlib, sys

col = sys.argv[1] if len(sys.argv) > 1 else "3x5領域か"
csv_path = pathlib.Path(__file__).resolve().parent.parent / "led-1d-coordinates.csv"
rows = list(csv.DictReader(csv_path.open(encoding="utf-8")))
if col not in rows[0]:
    sys.exit(f"列 {col!r} がない。利用可能: {list(rows[0])}")

sel = [int(r["index"]) for r in rows if r[col].strip() == "1"]
b = [0] * 8
for i in sel:
    b[i >> 3] |= 1 << (i & 7)

# バイト列から復元して CSV と一致することを確認
back = [i for i in range(60) if (b[i >> 3] >> (i & 7)) & 1]
assert back == sel, "マスクの生成に失敗"

print(f"// {col}: {len(sel)} LEDs -> {sel}")
print("#define RGB_3X5_MASK_BYTES " + ", ".join(f"0x{x:02X}" for x in b))
