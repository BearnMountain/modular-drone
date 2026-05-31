# App Information
- slamd — as mentioned, lightweight Python/C++ visualizer built for SLAM, with a transform tree and the right primitives (point clouds, camera frustums, etc.)
- ImGui + ImPlot — immediate-mode GUI, works great with SDL/OpenGL. Used widely in robotics for custom dashboards. ImPlot handles time-series data; you write your own map overlay.
- OpenCV — for the video/image pane.
- PCL (Point Cloud Library) — for SLAM map rendering if you need a full 3D point cloud pipeline.
- OGRE3D or bgfx — if you need a more capable render backend than raw SDL.

# Testing
## Datasets
Nothing here should be included in the app
- https://github.com/youngguncho/awesome-slam-datasets
