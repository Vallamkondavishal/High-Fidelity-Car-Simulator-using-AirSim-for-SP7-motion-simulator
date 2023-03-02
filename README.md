# High-Fidelity-Car-Simulator-using-AirSim-for-SP7-motion-simulator
Thesis project

## Introduction 
The automotive industry is rapidly evolving with new technologies like autonomous driving, electric vehicles, and advanced driver assistance systems. Developing high-fidelity car simulators has become increasingly essential for accurately simulating real-world scenarios and testing performance in controlled environments. This thesis aims to develop a highly configurable simulator using AirSim and Unreal Engine for SP7 motion simulator, offering an immersive and realistic driving experience for users. With potential applications such creating scenarios with AI characters by customizing their actions and researching advanced driver assistance systems, this simulator can lead to safer and more efficient vehicles on the road.

## Installation:

1. The initial step is to refer to the AirSim [documentation](https://microsoft.github.io/AirSim/build_windows/) .
2. Depending on the operating system, we must adhere to the appropriate installation procedure (in my case, I used Windows 11).
3. The following step is to install Visual Studio 2022 and integrate the required plugins.
4. Unreal Engine 4.27 is suitable for this project.
5. Once Unreal Engine has been installed, we must open it and establish a new C++ project.
6. After creating the project, we must duplicate some plugins from AirSim. The file path is AirSim > Unreal > Plugins, and they should be pasted into the newly created project.
7. Next, we should launch Visual Studio 2022 for the project and modify the project code that ends with .uproject according to the below-mentioned code.
8. After making the necessary changes, we can locate an appropriate asset from the Unreal Marketplace and add it to the newly created project.
9. Subsequently, all tabs should be closed, and the project should be reopened to apply the plugins to the project.
10. We should then open the level that will be utilized in the project.
11. Finally, after opening the project, we must select the windows option and activate the world setting option. Following that, we should modify the GameMode Overrider to AirSimGameMode.
