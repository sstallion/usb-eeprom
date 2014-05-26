# Protocol

A simple protocol was devised to provide chip-agnostic access to data. The
programmer is implemented as a vendor specific device; an additional host
driver is not required. EP1 serves as a bi-directional bulk transfer endpoint
that is used for protocol-level communication with the device. A generous
maximum packet size is also provided to enhance performance and support larger
page sizes.

## Commands
Every operation begins with a command (OUT) packet, which consists
of a single octet that indicates the command followed by zero or more octets
that serve as parameters. All multi-octet parameters are interpreted as
little-endian.

_Packets should contain at most one command; additional payload will be
discarded by the device._

### Read
A Read is indicated by 0x52 ('R'). Two parameters are required: the
starting address and length of data to be transferred. Both parameters are
16-bit unsigned integers. The device will respond with one or more data (IN)
packets containing the data.

### Byte Write
A Byte Write is indicated by 0x57 ('W'). Two parameters are
required: the starting address and length of data to be transferred. Both
parameters are 16-bit unsigned integers. One or more data (OUT) packets should
follow containing the data.

### Page Write
A Page Write is indicated by 0x50 ('P'). Two parameters are
required: the starting address and length of data to be transferred. Both
parameters are 16-bit unsigned integers. One or more data (OUT) packets should
follow containing the data. Data packets should be sized according to the page
size.

_Pages larger than the maximum packet size for the OUT endpoint are not
supported._

### Chip Erase
A Chip Erase is indicated by 0x5A ('Z').

## Status
Upon completion of a command, the device will respond with a status (IN) packet
that contains a 16-bit unsigned integer. For data transfer commands this will
be the next address to read or write. For a chip erase, this will always be
zero.

## Reset
A device reset may be issued to abort the current command. Resets should be
issued following a failed transfer to clear device state.

# Performance

Performance is largely dictated by the chip, however it can be interesting to
look at the relative difference between write modes. The measurements below
were collected using a superficial benchmark suite provided by the
github.com/sstallion/go/eeprom Go package. The chip under test was a 256Kbit
Atmel Parallel EEPROM (AT28C256):

    $ go test -v -bench . -benchtime 10ms github.com/sstallion/go/eeprom
    === RUN TestReset
    --- PASS: TestReset (0.26 seconds)
    === RUN TestVerify
    --- PASS: TestVerify (5.38 seconds)
            eeprom_test.go:62: verifying WriteBytes
            eeprom_test.go:62: verifying WritePages
    PASS
    BenchmarkRead8K                    1        151224468 ns/op
    BenchmarkWriteBytes8K              1        83607361736 ns/op
    BenchmarkWritePages8K              1        1364394100 ns/op
    BenchmarkErase                     1        21318916 ns/op
    ok      github.com/sstallion/go/eeprom      90.789s

_It should be noted that the timing for both write modes could be improved by
adding support for DATA polling._
