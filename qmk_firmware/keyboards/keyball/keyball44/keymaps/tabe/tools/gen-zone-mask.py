#!/usr/bin/env python3
"""led-1d-coordinates.csv の任意の 0/1 列から、config.h 用のビットマスクを生成する。

usage: python3 tools/gen-zone-mask.py [列名]   (既定: 3x5領域か)

出力される #define をそのまま config.h に貼る。チェーンインデックス gi に対する
判定は rgblight_call_driver 側の RGB_IS_3X5(gi) が行う。

値は半角の 0 / 1 のみを受け付ける。全角数字や空欄が混ざると「立っていない」と
黙って解釈されて気づけないため、エラーで止める。
"""
import csv, pathlib, sys, unicodedata

col = sys.argv[1] if len(sys.argv) > 1 else "3x5領域か"
csv_path = pathlib.Path(__file__).resolve().parent.parent / "led-1d-coordinates.csv"
rows = list(csv.DictReader(csv_path.open(encoding="utf-8")))
if col not in rows[0]:
    sys.exit(f"列 {col!r} がない。利用可能: {list(rows[0])}")

# 値の検証。半角 0/1 以外は、何が入っていたかを示して止める
bad = []
for r in rows:
    v = r[col].strip()
    if v not in ("0", "1"):
        cps = " ".join(f"U+{ord(c):04X}" for c in v) or "(空)"
        hint = ""
        if v and unicodedata.normalize("NFKC", v) in ("0", "1"):
            hint = f"  <- 全角のようです。半角の {unicodedata.normalize('NFKC', v)} に直してください"
        bad.append(f"  index={r['index']:>3}  値={v!r}  {cps}{hint}")
if bad:
    sys.exit(f"列 {col!r} に半角 0/1 以外の値があります:\n" + "\n".join(bad))

sel = [int(r["index"]) for r in rows if r[col] .strip()== "1"]
b = [0] * 8
for i in sel:
    b[i >> 3] |= 1 << (i & 7)

# バイト列から復元して CSV と一致することを確認
back = [i for i in range(60) if (b[i >> 3] >> (i & 7)) & 1]
assert back == sel, "マスクの生成に失敗"

# 左右の内訳も出す（非対称なゾーンは大抵入力ミス）
for side in ("左", "右"):
    s = [r for r in rows if r["右・左"] == side and r[col].strip() == "1"]
    cols = sorted({float(r["物理的な1D座標値"]) for r in s})
    print(f"// {side}: {len(s)} LEDs = {len(cols)}列 x {len(s)//len(cols) if cols else 0}行  座標={cols}")
print(f"// {col}: 計 {len(sel)} LEDs -> {sel}")
print("#define RGB_3X5_MASK_BYTES " + ", ".join(f"0x{x:02X}" for x in b))
