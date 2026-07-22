# Hardware — STM32 Multi-Protocol IMU Data Logger

Custom two-layer PCB for the STM32F446-based IMU data logger, designed in **Altium Designer** and
prepared for **JLCPCB** SMT assembly. See the [root README](../README.md) for the system overview,
schematic reasoning, and engineering calculations.

> **Revision 1:** design complete — ERC/DRC clean, Gerber/drill/BOM/CPL generated and reviewed for
> manufacturing. **Not yet fabricated, assembled, or electrically tested.**

## Layout

```
hardware/
├── altium/                 # Source design (open this in Altium)
│   ├── IMU sensor node hardware.PrjPcb      # project
│   ├── IMU_sensor_node_hardware.PcbDoc      # board (live PCB)
│   ├── Periferals.SchDoc / Support.SchDoc   # schematic sheets
│   ├── IMU sensor node hardware.BomDoc      # BOM document
│   ├── IMU sensor node hardware.OutJob      # output job (absolute paths — see caveat below)
│   └── libraries/          # libraries/schematic/ holds ONE SchLib; see "CAD models" below
├── manufacturing/
│   └── rev1/               # Reviewed release (what was checked against JLCPCB)
│       ├── gerbers.zip     # corrected key-shaped outline, copper/mask/paste/silk + NC drill
│       ├── bom.xlsx        # JLCPCB BOM (J1 excluded from PCBA)
│       ├── cpl.xlsx        # JLCPCB component placement list (J1 excluded)
│       └── SOURCING.md     # R1/R2 substitution + ordering notes
└── exports/
    ├── pcb-top.png                # Altium 3D render, top side (Rev 1)
    ├── schematic.pdf              # DRAFT/reference printout (not a clean final export)
    └── cpl-table-screenshot.png   # screenshot of the CPL spreadsheet (not a PCB image)
```

Altium-generated intermediates (`History/`, `__Previews/`, `Project Outputs/`, CAMtastic files) are
gitignored — the source design plus the reviewed `manufacturing/rev1/` release are the tracked
artifacts.

## Custom CAD models & library reproducibility

`libraries/schematic/` currently contains a **single** SchLib (the 2.2 nF capacitor symbol), and no
footprint libraries (`.PcbLib`) are committed. The other custom parts are **not** all present as
standalone library files:

- The **USB-C receptacle** symbol/footprint was hand-built; the **CH340C** and some capacitor models
  were imported via EasyEDA/LCSC and validated/adapted.
- Most of these live **embedded inside** `Support.SchDoc` / `Periferals.SchDoc` / the `.PcbDoc`, or
  originated from an **Altium 365 workspace server**, rather than as files in `libraries/`.

**Reproducibility limitation:** the board opens and renders from the committed source because the
models are embedded in the documents, but the project **cannot be fully library-updated / re-linked
from a clean machine** without the original Altium workspace (or rebuilding those library items).
Treat `libraries/` as partial, not the complete part library.

## Board specifications

- 2-layer FR-4, 1.6 mm, 1 oz outer copper, ENIG finish, green mask / white silk.
- ≈ 46.1 mm × 55.37 mm, tented vias, standard ±0.2 mm outline tolerance.
- JLCPCB Economic PCBA, top-side assembly. Parts chosen JLCPCB-**Basic** where possible; 4 Extended
  parts (STM32, MPU-6050, USB-C, CH340C).

## Reproducing the manufacturing outputs

1. Open `altium/IMU sensor node hardware.PrjPcb` in Altium Designer.
2. Run the OutJob (`IMU sensor node hardware.OutJob`) to regenerate Gerbers, NC drill, BOM, and CPL.
3. The reviewed Revision 1 set is committed under `manufacturing/rev1/` for reference.

> **OutJob portability caveat:** `IMU sensor node hardware.OutJob` hard-codes absolute Windows paths
> (`C:\Users\Public\Documents\Altium\IMU sensor node hardware\…`) for its source PCB and PDF output
> targets. These were left as-is deliberately — Altium's portable-path encoding depends on per-output
> "relative to project" flags that can't be set safely by editing the file directly. **After cloning
> on a different machine, reassign the OutJob's source and output paths inside Altium** before
> regenerating, and verify the output targets.

## Assembly notes

- **R1/R2 (I²C pull-ups):** the BOM's manufacturer part number and the assembled LCSC part differ by
  design — see [`manufacturing/rev1/SOURCING.md`](manufacturing/rev1/SOURCING.md) for the substitution
  and why it's compatible.
- **J1 (SWD header):** intentionally excluded from the JLCPCB BOM/CPL (through-hole part, avoids
  assembly cost). The footprint is on the board — hand-solder a 1×5, 2.54 mm vertical male header.
- **MPU-6050:** obsolete but still JLCPCB-stocked; retained for Rev 1, to be replaced with a current
  IMU in Rev 2. Its exposed pad is **not** soldered and has no vias/routing beneath it; a continuous
  bottom GND plane under the package provides shielding.
- Recheck live JLCPCB stock, Basic/Extended tiers, and pricing before ordering — availability drifts.

## Pending (Revision 1)

Fabrication, assembly, and hardware bring-up (power-rail, short-circuit, SWD, USB-UART, I²C, SPI, and
firmware validation) are the remaining steps. Results will be recorded under `docs/verification/`.

Export work still pending in Altium:
- The **bottom-side** board render (`pcb-bottom.png`). The top-side 3D render (`exports/pcb-top.png`)
  is included.
- A **clean schematic-only PDF**. The current `exports/schematic.pdf` is a draft/reference only —
  page 2 is clipped, title blocks are incomplete, and the PCB page is small with excess whitespace.
