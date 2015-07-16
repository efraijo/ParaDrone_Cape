# Background: 

Recently, many kinds of unmanned aerial vehicles have been researched and developed for various purposes such as land observation, surveillance, and reconnaissance of the grounds circumstances. Most of them are fixed/rotary-wing aircraft, which are too heavy and large to carry equipment for observation and communications. Another type of UAV, an airship or an air balloon, also has a larger airframe in comparison with their payload. However, rotary-wing aircraft and airships lack portability due to their weight and size. In contrast, a powered paraglider is excellent in portability, since it is folded on the ground, and unfolded when it flies. Although a paraglider is subject to wind disturbances, the portability justifies the use of a paraglider as a UAV, for example the surveying of large archeological sites around the world. 

In addition to the portability of the Paraglider, its low flight speed and high endurance is suitable for detailed land observation as well as long distance coverage. Furthermore, a powered paraglider is safe and survivable in case of engine failure, since it can continue a stable gliding flight without thrust and can be guided to a safe landing point. A paraglider provides you with a portable, safe, long endurance aircraft that can be used to survey small and large sites of interest. The paraglider has a range of applications in the sectors of agriculture, mining, constructions, and academia. The availability to carry a heavy modular payload in one platform makes the paraglider a risk averse aerial platform in comparison to the fixed/rotary-wing platforms. 

The idea is to automate the powered paraglider and be able to facilitate aerial data acquisition. It will enable researchers the ability to interact and explore their surroundings with a risk averse platform

# Project Overview:
 
The powered paraglider is a platform that will be able to navigate aerial space providing high stability and long endurance. The main purpose of this project is to provide researchers with a stable modular aerial platform. It will enable scientist to accommodate visualization and communication payloads in order to generate panoramic pictures at high resolutions. One application of the project is to compile aerial images to generate 3D representation of the surrounding environment. Another application of the project will accommodate a communication payload to determine the location of VHF radio collars. 

# Project Approach: 
 
The project has three sectors: mechanical, software, and control design. The mechanical design work is being developed by Elioth Fraijo, Miguel De la Villa, and Dominique Meyer. The mechanical design will consist of multiple iterations for which the final design iteration will allow a series of payloads to be used in the paraglider. The software development and sensor integration will be done by Jerry Wang and Elioth Fraijo. Jerry Wang will be focusing on developing a high-level control software. Elioth Fraijo will also parametrize the system behavior and develop a discrete control system. 

The mechanical design has several components: design, assembly, and testing. The mechanical design will follow a flow chart design style, where one iteration will pass through the design phase onto the assembly and go through testing. The current paraglider design is on its second iteration. The significant design changes that were done for the second iteration include: fortification on the propeller strut connection points, placement of the propelling motor, and integration of carbon fiber tubes to increase the connection points for the canopy to the airframe. The third iteration is expected to integrate visual and communication payloads as well as to facilitate the assembly of the airframe. Testing will be done periodically once or twice a week to determine system stability and endurance. 

The software development is made of two components: sensor integration and high-level control software. The sensor integration will be done by compiling all the hardware data sheets and obtaining all the drivers needed to communicate with the onboard computer. If no drivers are available, Jerry Wang will write the drivers needed for system completion. The high level control software will be modeled after James Strawson’s MiP controller found at https://github.com/StrawsonDesign?tab=repositories.

The control software for the automation of the paraglider has several components: real-time control and telemetry management. Real-time control consists of a steady level flight, altitude flight, and waypoint navigation control. Telemetry consists of synchronization of the avionics sensors and detailed navigation data to the ground. Telemetry management software will be developed in two phases. First, the telemetry data will be collected and post-processed for analysis. Second, the telemetry data will be packaged and sent to the ground to provide the pilot real-time navigation data. 

# Constrain, Risk, And Feasibility:

As the mechanical airframe is under development, and the testing of the system requires actual time in air, there are several potential setbacks. The airframe manufacturing and assembly has a duration of 4-6 days, which is a significant delay in development. If the airframe requires a redesign, control development would be delayed for 1-2 weeks. Since none of the group members are familiar with flying a remote controlled paraglider, there is a possibility of pilot error. 

Sensor integration and software development is more likely to succeed. Sensor integration is independent of mechanical testing and requires no actual time in air. The software development also has autonomy of hardware testing and data gathering for the control dynamics. 

# Group Management:

As our group consists of five members, a project manager has been assigned to meet and discuss all aspects of the project development with team members. Since our project is divided in three sectors, there will be separate meeting times for each focus of the project. A joint meeting between all members will take place once a week to discuss project development. To manage the work schedule, we will create a weekly to-do list. In the given case that we fall behind schedule, we will evaluate the reasons behind the delay, and devote more resources to those specific parts of the project, or redefine the scope of the project to accommodate for the 10 week constraints of the quarter. 


# paraglide
Beaglebone BLACK code for Auto Paraglider project
Aquiring data from Robotics cape and external sensors. Logging realtime data into csv form file.

bmp180:     driver for internal barometer on Robotics cape
global:     public settings
log:        logging module
paraglide:  main body
task:       every second tasks
ubloxlea6:  external GPS reciever driver (unfinished yet)
