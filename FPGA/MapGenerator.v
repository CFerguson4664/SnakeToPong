// Christopher Ferguson, Elliot Wannemacher
// ECE361 Lab Project
// Spring 2021
//
// Modified by Christopher Ferguson
// For ECE371
// Fall 2022

module MapGenerator(row,data,toggle,command,mult);
	input[1:0] mult;
	input[23:0] command;

	// On positive edge apply data
	input toggle;
	
	// Which row of the image we are displaying
	input[8:0] row;
	
	// Row output
	output reg[5087:0] data;
	
	// Create the registers to store the frame data
	reg[335:0] frame1[31:0];
	
	// The row to display
	reg[31:0] rowD;
	
	wire SRout;
	wire[7:0] x,y;
	wire[5:0] inData;
	
	assign x = command[23:16];
	assign y = command[15:8];
	assign inData = command[5:0];
	
	//assign rs = SRout & clk50;
	
	//SRLatch SRLch(toggle, rs, SRout);
	
	always@(posedge toggle)
	begin
		case(mult)
		2'd0: begin
					frame1[y][(x * 6)+:6] <= inData;
				end
		2'd1: begin
					frame1[y][(x * 12)+:12] <= {2{inData}};
				end
		2'd3: begin
					frame1[y][(x * 24)+:24] <= {4{inData}};
				end
		endcase
	end
	
	always@(*)
	begin
		rowD = row / (15 * (mult + 1));
		data = {24'd0,	{15{frame1[rowD][335:330]}},
							{15{frame1[rowD][329:324]}},
							{15{frame1[rowD][323:318]}},
							{15{frame1[rowD][317:312]}},
							{15{frame1[rowD][311:306]}},
							{15{frame1[rowD][305:300]}},
							{15{frame1[rowD][299:294]}},
							{15{frame1[rowD][293:288]}},
							{15{frame1[rowD][287:282]}},
							{15{frame1[rowD][281:276]}},
							{15{frame1[rowD][275:270]}},
							{15{frame1[rowD][269:264]}},
							{15{frame1[rowD][263:258]}},
							{15{frame1[rowD][257:252]}},
							{15{frame1[rowD][251:246]}},
							{15{frame1[rowD][245:240]}},
							{15{frame1[rowD][239:234]}},
							{15{frame1[rowD][233:228]}},
							{15{frame1[rowD][227:222]}},
							{15{frame1[rowD][221:216]}},
							{15{frame1[rowD][215:210]}},
							{15{frame1[rowD][209:204]}},
							{15{frame1[rowD][203:198]}},
							{15{frame1[rowD][197:192]}},
							{15{frame1[rowD][191:186]}},
							{15{frame1[rowD][185:180]}},
							{15{frame1[rowD][179:174]}},
							{15{frame1[rowD][173:168]}},
							{15{frame1[rowD][167:162]}},
							{15{frame1[rowD][161:156]}},
							{15{frame1[rowD][155:150]}},
							{15{frame1[rowD][149:144]}},
							{15{frame1[rowD][143:138]}},
							{15{frame1[rowD][137:132]}},
							{15{frame1[rowD][131:126]}},
							{15{frame1[rowD][125:120]}},
							{15{frame1[rowD][119:114]}},
							{15{frame1[rowD][113:108]}},
							{15{frame1[rowD][107:102]}},
							{15{frame1[rowD][101:96]}},
							{15{frame1[rowD][95:90]}},
							{15{frame1[rowD][89:84]}},
							{15{frame1[rowD][83:78]}},
							{15{frame1[rowD][77:72]}},
							{15{frame1[rowD][71:66]}},
							{15{frame1[rowD][65:60]}},
							{15{frame1[rowD][59:54]}},
							{15{frame1[rowD][53:48]}},
							{15{frame1[rowD][47:42]}},
							{15{frame1[rowD][41:36]}},
							{15{frame1[rowD][35:30]}},
							{15{frame1[rowD][29:24]}},
							{15{frame1[rowD][23:18]}},
							{15{frame1[rowD][17:12]}},
							{15{frame1[rowD][11:6]}},
							{15{frame1[rowD][5:0]}}, 24'd0};
					
	end
endmodule
