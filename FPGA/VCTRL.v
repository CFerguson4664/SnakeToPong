// Christopher Ferguson, Elliot Wannemacher
// ECE361 Lab Project
// Spring 2021

module VCTRL(CLK,SYNC,CNT,HCNT);
	input CLK;
	input[10:0] HCNT;
	output SYNC;
	output reg[9:0] CNT;
	wire CLRV;
	
	//Once the counter hits 517, reset it
	assign CLRV = (CNT <= 517) ? 1'b0 : 1'b1;
	
	//SYNC is active low. Set it low when we are in the vertical sync interval
	assign SYNC = ((CNT >= 7) & (CNT <= 15)) ? 1'b0 : 1'b1;
	
	always@(posedge CLK)
	begin
		if(CLRV) CNT <= 0;
		else
		begin
			if(HCNT == 1086) CNT <= CNT + 10'd1;
		end
	end
endmodule
