# tcleaner
(c) 2018 Jacob Bogner

This project is a complete cleaning assistant for the Taylor Company C602/C606 combination shake/soft serve freezers, also potentially applicable to the C716/C717 soft serve freezers. Project has 2 categories, hardware and software.

Hardware:

A) The primary feature is a multi-use water distribution and waste management apparatus. Mounted on casters to ensure portability. Requires 1 supply, 1 drain, and 1 AC 120V connection. Features include
  1. Water supply valves to supply a cleaning hose or hopper cleaning manifold (see ) for the purposes of cleaning appliances. In the event a city hookup is not available, water can be drawn from supply bucket via pump. When a city supply is availble the supply bucket doubles as a special solution bucket and can supply sanitizer or approved cleaners to cleaning manifold or cleaning hose. Also has applications in certain frappe machine/drain/floor cleaning applications.
  2. Draining system with several connect points to route all waste through a central drain hose. Compressor provides a means to purge this line in the event it becomes blocked.
  3. Drying/Purging pneumatic system powered by a central air compressor can supply low-pressure air for drain purging/equipment drying via handheld tool accessory. Can also internally reroute air through water supply system to purge clean and waste water from internal plumbing to allow storage without risk of mold/buildup
  4. Hopper filling manifold mounts ontop of the C602/C606 hopper shell and provides automatically controlled valves to fill mix hoppers with fresh water/cleaning solutions. 2 level-full probes are provided to allow the flow of fluid to be automatically shut off when the hopper is filled.
  5. Draining manifold mounts directly underneath C602/C606 draw valves to allow continuous machine draining. Connects via fittings directly to drain connections on cleaning apparatus
  
Software

B) There are 2 independent controllers connected via serial link:
  1. The Controller unit shall be an Arduino MEGA board and provide appropriate inputs and outputs to control and monitor the various hardware components.
  2. The UI board shall be an Arduino UNO board driving a 4" touch display and provide communication with Controller via serial link.
