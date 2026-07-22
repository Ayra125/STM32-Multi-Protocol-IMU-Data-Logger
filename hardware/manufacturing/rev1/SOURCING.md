# Revision 1 — sourcing notes

Notes on the released BOM (`bom.xlsx`) / CPL (`cpl.xlsx`) for JLCPCB assembly. JLCPCB places parts
by the **LCSC part number** (the `C#####` in the `LCSC Part #` column), so that column — not the
`Manufacturer Part Number` column — determines the part actually assembled.

## Intentional substitution — R1 / R2 (I²C pull-ups)

| BOM field | Value |
|---|---|
| Designators | R1, R2 |
| `Comment` / `Manufacturer Part Number 1` | Resistor 2.2 kΩ ±5% 0402 63 mW / **CR0402-JW-222GLF** (Bourns) |
| `LCSC Part #` (what gets placed) | **C25879** = `0402WGF2201TCE` (UNI-ROYAL), 2.2 kΩ **±1%** 0402, 62.5 mW |

**What this means:** the `Manufacturer Part Number` column still carries the original Altium library
entry (Bourns `CR0402-JW-222GLF`, ±5%), while the linked LCSC part `C25879` is the deliberately
chosen UNI-ROYAL `0402WGF2201TCE` (±1%). The assembled resistor is therefore the ±1% UNI-ROYAL part.

**Why the substitute is compatible:**
- Same resistance (2.2 kΩ) and same package (0402) — mechanically and electrically drop-in.
- Tighter tolerance (±1% vs. ±5%) — strictly better for the I²C rise-time budget, never worse.
- Well inside the derived valid pull-up window of ≈ 967 Ω – 17.6 kΩ (see the root README's
  engineering-calculations section).
- `C25879` is a JLCPCB **Basic** part (no per-part fee), which is why it was selected over the
  original MPN.

**Do not "fix" the MPN column by editing `bom.xlsx`.** JLCPCB keys off the LCSC `C25879`; changing the
MPN text is unnecessary and risks corrupting the upload workbook. This note is the record of intent.

## Before ordering

Re-check live JLCPCB stock, the Basic/Extended tier, and pricing for every line — availability and
tiers drift over time. The MPU-6050 (`C24112`) in particular is obsolete but currently JLCPCB-stocked;
confirm it is still assemblable at order time.
