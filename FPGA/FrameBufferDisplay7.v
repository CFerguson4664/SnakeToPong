// Christopher Ferguson, Elliot Wannemacher
// ECE361 Lab Project
// Spring 2021

module FrameBufferDisplay7(
	CLOCK_50, 						// Input 50 MHz Clock
	VGA_HS,							// VGA Horizontal Sync Signal
	VGA_VS,							//	VGA Vertical Sync Signal
	VGA_BLANK_N,					// VGA Blanking Signal
	VGA_SYNC_N,						// VGA Sync Signal
	VGA_R,							// VGA Red Value
	VGA_G,							// VGA Green Value
	VGA_B,							// VGA Blue Value
	VGA_CLK,							// VGA Output Clock
	
	HEX0,
	HEX1,
	HEX2,
	HEX3,
	HEX4,
	HEX5,
	LEDR,
	KEY,
	SW,
	GPIO
);			

//Control inputs
input[3:0] KEY;
input[9:0] SW;
input[2:0] GPIO;

//Debug outputs
output[6:0] HEX0,HEX1,HEX2,HEX3,HEX4,HEX5;
output[9:0] LEDR;

//Clock input
input CLOCK_50;

//VGA outputs
output VGA_HS,VGA_VS,VGA_BLANK_N,VGA_SYNC_N,VGA_CLK;
output[7:0] VGA_R,VGA_G,VGA_B;

reg switchBuffer = 0;

//VGA internal Controls
wire[10:0] HCount;
wire[8:0] VCount;
reg[10:0] HPixel;
reg[8:0] VPixel;


//Storage for the VGA display code
reg[1695:0] currentLine;
reg[1695:0] nextLine;
wire[1695:0] mapData;

//Wires for the clock signals.
wire pixelCLK,logicCLK;
wire PLLRST;

//Registers to store the color to display	
reg[7:0] red = 0,blue = 0,green = 0;
	
//PLL generates the signal for the VGA pixel clock
	PLL p0 (CLOCK_50,PLLRST,pixelCLK);
	
assign logicCLK = pixelCLK;

// HCTRL and VCTRL drive the VGA output
// HCTRL		  (CLK,      SYNC, CNT)
	HCTRL CTRL0(pixelCLK, VGA_HS, HCount);
	
// VCTRL		  (CLK,      SYNC,   CNT,    HCNT)
	VCTRL CTRL1(pixelCLK, VGA_VS, VCount, HCount);
	
//Calculate when we are finished displaying the current line
assign newLine = (HCount == 1086) ? 1'b1 : 1'b0;

//Control the VGA blanking interval based on HCTRL and VCTRL	
assign VGA_BLANK_N = ((VCount > 37) & (VCount <= 517) & (HCount > 240) & (HCount <= 1088)) ? 1'b1 : 1'b0;

//Assign the colors to their outputs
assign VGA_R = red;
assign VGA_G = green;
assign VGA_B = blue;

//Assign the clock signals to their outputs
assign VGA_CLK = pixelCLK;
assign VGA_SYNC_N = 1'b0;

wire ready;

//
//MapGenerator provides all of the data reguarding the map
MapGenerator mp(VPixel,mapData,SW[0],ready,switchBuffer,x,y,data);

wire[7:0] x,y,data;

SPMod sp(HEX0,HEX1,HEX2,HEX3,HEX4,HEX5,GPIO[0],GPIO[1],GPIO[2],x,y,data,ready);

assign LEDR[2:0] = GPIO[2:0];


//Main control loop for the system
always@(posedge logicCLK)
begin
	//Calculate the horizontl pixel which is currently being displayed.
	if(((HCount > 240) && (HCount <= 1088)))
	begin
		HPixel <= HPixel + 11'd1;
	end 
	
	
	//If we are beginning a new line
	if(newLine == 1)
	begin
		//Reset the control counters
		HPixel <= 0;
		currentLine <= mapData;
		switchBuffer <= 1;
		
		//If we are starting a new frame
		if(VCount == 0)
		begin
			//Counter to keep track of what vertical line is being displayed
			VPixel <= 0;
		end else if(((VCount > 33) && (VCount <= 513)))
		begin
			VPixel <= VPixel + 9'd1;
		end 
	end else begin
		switchBuffer <= 0;
	end
	
	//Convert the data from MapGenerator into actual colors
	case(currentLine[(HPixel * 2)+:2])
	0	:	begin
				red = 0;
				green = 0;
				blue = 0;
			end
	1	:	begin
				red = 255;
				green = 0;
				blue = 0;
			end
	2	:	begin
				red = 0;
				green = 255;
				blue = 0;
			end
	3	:	begin
				red = 0;
				green = 0;
				blue = 255;
			end
	endcase
end

endmodule
