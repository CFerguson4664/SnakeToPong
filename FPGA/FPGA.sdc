create_clock -name "CLOCK_50" -period 20.000ns [get_ports CLOCK_50]
create_clock -name "KEY[0]" -period 20.000ns [get_ports KEY[0]]
derive_pll_clocks
derive_clock_uncertainty