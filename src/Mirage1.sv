module Mirage1(
    input   bit aClock,
    input   bit aReset,

    output [7:0] anOutPortA,
    output [7:0] anOutPortB
);

wire [15:0] address;
wire [15:0] dataIn;
wire [15:0] dataOut;
wire write;
wire read;

RAM_1R_1W 
#(
	.DEPTH(16),
	.SIZE(4095)
)
mem_inst
(
    .aClock(aClock),
	.aReadAddress(address[11:0]),
	.anOutReadData(dataIn),
	.aReadEnable(!write),
	.aWriteAddress(address[11:0]),
	.aWriteData(dataOut),
	.aWriteEnable(write)
);

RISC16 risc16_inst
(
    .aClock(aClock),
    .aReset(aReset),
    .anOutAddress(address),
    .aData(dataIn),
    .anOutData(dataOut),
    .anOutWrite(write)
);

always_comb begin
    if(address == 16'h0200 && write) begin
        anOutPortA = dataOut[7:0];
        read = 0;
    end
    else if(address == 16'h0201 && write) begin
        anOutPortB = dataOut[7:0];
        read = 0;
    end
    else begin
        read = 1;
    end
end


endmodule // Mirage1