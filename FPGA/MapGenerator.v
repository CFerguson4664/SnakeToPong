// Christopher Ferguson, Elliot Wannemacher
// ECE361 Lab Project
// Spring 2021
//
// Modified by Christopher Ferguson
// For ECE371
// Fall 2022

module MapGenerator(row,data,map,toggle,switchBuffer,x,y,dataIn);

	input[7:0] x,y,dataIn;

	input switchBuffer;

	// On negative edge switch frame buffers
	input toggle;
	
	// Which frame buffer to use 
	input map;
	
	// Which row of the image we are displaying
	input[8:0] row;
	
	// Row output
	output reg[1695:0] data;
	
	// Create the registers to store the frame data
	reg[13:0] frame1[7:0];
	reg[13:0] frame2[7:0];
	
	// The frame currently being displayed
	reg mapA = 0;
	
	// The row to display
	reg[31:0] rowD;
	
	wire sbuf = toggle & switchBuffer;
	
	initial begin
		//Initialize the data for each of the maps
		//A '1' is a wall and a '0' is empty space
		frame1[0] = 14'b11111111111111;
		frame1[1] = 14'b11111111111111;
		frame1[2] = 14'b11111111111111;
		frame1[3] = 14'b11111111111111;
		frame1[4] = 14'b11111111111111;
		frame1[5] = 14'b11111111111111;
		frame1[6] = 14'b11111111111111;
		frame1[7] = 14'b11111111111111;
		
		frame2[0] = 14'b11111111111111;
		frame2[1] = 14'b11111111111111;
		frame2[2] = 14'b11111111111111;
		frame2[3] = 14'b11111111111111;
		frame2[4] = 14'b11111111111111;
		frame2[5] = 14'b11111111111111;
		frame2[6] = 14'b11111111111111;
		frame2[7] = 14'b11111111111111;
	end
	
	always@(posedge sbuf)
	begin
		case(mapA)
		1'b0: begin
					frame1[y][10 - x] <= dataIn[0];
				end
		1'b1: begin
					frame2[y][10 - x] <= dataIn[0];
				end
		endcase
	
		mapA <= mapA ^ 1'b1;
	end
	
	always@(*)
	begin
		//Send the correct data for the map we are doing
		case(mapA)
		1'b0 :	begin
						rowD = row / 60;
						data = {8'd0,	{120{frame1[rowD][0]}},
											{120{frame1[rowD][1]}},
											{120{frame1[rowD][2]}},
											{120{frame1[rowD][3]}},
											{120{frame1[rowD][4]}},
											{120{frame1[rowD][5]}},
											{120{frame1[rowD][6]}},
											{120{frame1[rowD][7]}},
											{120{frame1[rowD][8]}},
											{120{frame1[rowD][9]}},
											{120{frame1[rowD][10]}},
											{120{frame1[rowD][11]}},
											{120{frame1[rowD][12]}},
											{120{frame1[rowD][13]}},8'd0};
					end
		1'b1 :	begin
						rowD = row / 60;
						data = {8'd0,	{120{frame2[rowD][0]}},
											{120{frame2[rowD][1]}},
											{120{frame2[rowD][2]}},
											{120{frame2[rowD][3]}},
											{120{frame2[rowD][4]}},
											{120{frame2[rowD][5]}},
											{120{frame2[rowD][6]}},
											{120{frame2[rowD][7]}},
											{120{frame2[rowD][8]}},
											{120{frame2[rowD][9]}},
											{120{frame2[rowD][10]}},
											{120{frame2[rowD][11]}},
											{120{frame2[rowD][12]}},
											{120{frame2[rowD][13]}},8'd0};
					end
		endcase
	end
endmodule
