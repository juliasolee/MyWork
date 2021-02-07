# Gouraud and Phong Illumination

## Main Concept
In this project we learned how to work with the GPU to implement shaders. We applied Gouraud shading by creating a shader that computed vertex illumination and Phong shading by creating another shader that computed pixel illumination. We also applied blending so we were able to use multiple lights, and camera movement to enable interaction. Then, we improved the Phong shader (that used information per pixel) to apply textures.

This project was developed in C++ and OpenGL. It was done for the UPF course Computer Graphics.

## Controls and functionalities

Keys W, A, S and D: move the camera (eye vector) Keys UP, DOWN, LEFT and RIGHT: move the light (light 1)

Change materials: Key 1: Plastic Key 2: Gold Key 3: Argent Key 4: Rubber Key 5: Texture

Change shading: P: Phong (default) G: Gouraud

Change light color: Key 7: Red Key 8: Green Key 9: Blue

Change light components: Keys Z + UP: Increase ambient Keys Z + Down: Decrease ambient Keys X + UP: Increase diffuse Keys X + Down: Decrease diffuse Keys C + UP: Increase specular Keys C + Down: Decrease specular

Two lights: Key H: Turn light 2 on Key O: Turn light 2 off Keys UP, DOWN, LEFT and RIGHT + L: Move light 2

Key 0: Reset

## Acknowledgements and Contributions

**Developers:** Júlia Solé Cubilo, Irene Ubieto Alamillo

**Framework Development and Assistance:** Javi Agenjo
