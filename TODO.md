# TODO

## Board Updates

- [x] Verify series resistors on D+/D-
- [ ] Introduce 500mA load switch for 5V_OUT (LED and ZIF) (TI TPS2010?)
- [ ] Re-route differential pair for USB
- [ ] Increase drill for ICSP and XU1
- [ ] Reduce distance between R2 and !MCLR pin (250 mils recommended)
- [ ] Consider changing LED color
- [ ] Better R value for LED to reduce brightness
- [ ] Choose better footprint for D1; fix label position
- [ ] Consider dual ground planes (remove power plane)
- [ ] Introduce power net classes
- [ ] Resolve implicit net names
- [ ] Revise layout (add drill and stacking info to documentation layer)
- [ ] Change GitHub link in silk screen to HTTPS
- [ ] Revise schematic (update frame and break into logical units)
- [ ] Change DRC to SilverCircuits compatible design
- [ ] Update BOM (via properties)
- [ ] Change primary supplier to DigiKey
- [ ] Create BOM spreadsheet for upload to DigiKey BOM Management
- [ ] Generate Gerber files for manufacturing
- [ ] Prototype run via OSHPark, SilverCircuits for production
- [ ] Order stencil for production run
- [ ] Investigate stencil rig to simplify paste application

## Software Updates

- [x] Increase maximum current to 500mA in USB config descriptor
- [ ] Remove USB reset hack; substitute with reset command
- [ ] Support USB SUSPEND/RESUME via sleep modes
- [ ] Better error reporting from device to client
- [ ] Consider adding power control to protocol
- [ ] Verify PIC18F config options; enable watchdog
- [ ] Rewrite client in C (autotools?)

## Other

- [ ] Changes are large enough to warrant a 2.0 release; C client will not
      be compatible with older board revisions
- [ ] General cleanup of sources and documentation
