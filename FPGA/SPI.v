module SPMod(HEX0,HEX1,HEX2,HEX3,HEX4,HEX5,SPI_CLK,SPI_CS,SPI_DATA,command,ready);
	output[6:0] HEX0,HEX1,HEX2,HEX3,HEX4,HEX5;
	input SPI_CLK,SPI_CS,SPI_DATA;
	
	output ready;
	
	reg[7:0] x,y,data;
	
	output reg[23:0] command = 0;
	
	SSHEX h5(x[7:4],HEX5);
	SSHEX h4(x[3:0],HEX4);
	
	SSHEX h3(y[7:4],HEX3);
	SSHEX h2(y[3:0],HEX2);
	
	SSHEX h1(data[7:4],HEX1);
	SSHEX h0(data[3:0],HEX0);
	
	assign ready = SPI_CS;
	
	always@(negedge SPI_CLK)
	begin
		command <= command << 1;
		command[0] = SPI_DATA;
	end
	
	always@(posedge SPI_CS)
	begin
		x <= command[23:16];
		y <= command[15:8];
		data <= command[7:0];
	end
endmodule