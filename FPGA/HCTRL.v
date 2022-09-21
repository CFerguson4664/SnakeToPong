// Christopher Ferguson, Elliot Wannemacher
// ECE361 Lab Project
// Spring 2021

module HCTRL(CLK,SYNC,CNT);
	input CLK;
	output SYNC;
	output reg[10:0] CNT;
	wire CLRH;
	
	//Once the counter hits 1088, reset it
	assign CLRH = (CNT <= 1088) ? 1'b0 : 1'b1;
	
	//SYNC is active low. Set it low when we are in the horizontal sync interval
	assign SYNC = ((CNT >= 16) & (CNT <= 128)) ? 1'b0 : 1'b1;
	
	always@(posedge CLK)
	begin
		if(CLRH) CNT <= 0;
		else CNT <= CNT + 11'd1;
	end
endmodule
