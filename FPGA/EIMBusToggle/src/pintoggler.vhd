library ieee;
use ieee.std_logic_1164.all;

entity pintoggler is
	port (
		clk_50	: in	std_logic;
		data			: out	std_logic_vector(15 downto 0);
		addr		: out std_logic_vector(15 downto 0);
		div		: out std_logic_vector(15 downto 0)
	);
end entity;


architecture rtl of pintoggler is

	signal counter : integer   := 0;
	signal toggler : std_logic_vector(15 downto 0) := (others => '0');
	
	constant clk_ticks : integer := 5000000;
	
begin

	data <= toggler;
	addr <= toggler;
	

	count: process (clk_50)
	begin
		if (rising_edge(clk_50)) then
			if ((counter mod clk_ticks) = 0)			then toggler(0) <= not toggler(0); end if;
			if ((counter mod (clk_ticks/2)) = 0)	then toggler(1) <= not toggler(1); end if;
			if ((counter mod (clk_ticks/4)) = 0)		then toggler(2) <= not toggler(2); end if;
			if ((counter mod (clk_ticks/8)) = 0)		then toggler(3) <= not toggler(3); end if;
			if ((counter mod (clk_ticks/16)) = 0)		then toggler(4) <= not toggler(4); end if;
			if ((counter mod (clk_ticks/32)) = 0)		then toggler(5) <= not toggler(5); end if;
			if ((counter mod (clk_ticks/64)) = 0)		then toggler(6) <= not toggler(6); end if;
			if ((counter mod (clk_ticks/128)) = 0)	then toggler(7) <= not toggler(7); end if;
			if ((counter mod (clk_ticks/256)) = 0)	then toggler(8) <= not toggler(8); end if;
			if ((counter mod (clk_ticks/512)) = 0)	then toggler(9) <= not toggler(9); end if;
			if ((counter mod (clk_ticks/256)) = 0)	then toggler(10) <= not toggler(10); end if;
			if ((counter mod (clk_ticks/128)) = 0)	then toggler(11) <= not toggler(11); end if;
			if ((counter mod (clk_ticks/64)) = 0)	then toggler(12) <= not toggler(12); end if;
			if ((counter mod (clk_ticks/32)) = 0)	then toggler(13) <= not toggler(13); end if;
			if ((counter mod (clk_ticks/16)) = 0)	then toggler(14) <= not toggler(14); end if;
			if ((counter mod (clk_ticks/8)) = 0)	then toggler(15) <= not toggler(15); end if;
			
			if (counter > clk_ticks) then
				counter <= 0;
			else
				counter <= (counter + 1);
			end if;
		end if;
	end process;

end architecture;
