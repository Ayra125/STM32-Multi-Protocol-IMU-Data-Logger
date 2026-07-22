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
│   ├── IMU sensor node hardware.OutJob      # output job (Gerbers/drill/BOM/CPL)
│   └── libraries/          # custom symbols/footprints (schematic/, footprints/)
├── manufacturing/
│   └── rev1/               # Reviewed release (what was checked against JLCPCB)
│       ├── gerbers.zip     # corrected key-shaped outline, copper/mask/paste/silk + NC drill
│       ├── bom.xlsx        # JLCPCB BOM (J1 excluded from PCBA)
│       └── cpl.xlsx        # JLCPCB component placement list (J1 excluded)
└── exports/
    ├── schematic.pdf                # schematic/board printout
    └── pcb-placement-preview.png    # JLCPCB top-side placement preview
```

Altium-generated intermediates (`History/`, `__Previews/`, `Project Outputs/`, CAMtastic files) are
gitignored — the source design plus the reviewed `manufacturing/rev1/` release are the tracked
artifacts.

## Board specifications

- 2-layer FR-4, 1.6 mm, 1 oz outer copper, ENIG finish, green mask / white silk.
- ≈ 46.1 mm × 55.37 mm, tented vias, standard ±0.2 mm outline tolerance.
- JLCPCB Economic PCBA, top-side assembly. Parts chosen JLCPCB-**Basic** where possible; 4 Extended
  parts (STM32, MPU-6050, USB-C, CH340C).

## Reproducing the manufacturing outputs

1. Open `altium/IMU sensor node hardware.PrjPcb` in Altium Designer.
2. Run the OutJob (`IMU sensor node hardware.OutJob`) to regenerate Gerbers, NC drill, BOM, and CPL.
3. The reviewed Revision 1 set is committed under `manufacturing/rev1/` for reference.

## Assembly notes

- **J1 (SWD header):** intentionally excluded from the JLCPCB BOM/CPL (through-hole part, avoids
  assembly cost). The footprint is on the board — hand-solder a 1×5, 2.54 mm vertical male header.
- **MPU-6050:** obsolete but still JLCPCB-stocked; retained for Rev 1, to be replaced with a current
  IMU in Rev 2. Its exposed pad is **not** soldered and has no vias/routing beneath it; a continuous
  bottom GND plane under the package provides shielding.
- Recheck live JLCPCB stock, Basic/Extended tiers, and pricing before ordering — availability drifts.

## Pending (Revision 1)

Fabrication, assembly, and hardware bring-up (power-rail, short-circuit, SWD, USB-UART, I²C, SPI, and
firmware validation) are the remaining steps. Results will be recorded under `docs/verification/`.
Board renders (`pcb-top.png`, `pcb-bottom.png`, `pcb-3d.png`) are still to be exported from Altium.
