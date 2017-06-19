# Deferred Voxel Shading for Real Time Global Illumination

*Peer-review published paper for this technique can be found here:* http://ieeexplore.ieee.org/abstract/document/7833375/

Computing indirect illumination is a challenging and complex problem for real-time rendering in 3D applications. This global illumination approach computes indirect lighting in real time utilizing a simpliﬁed version of the outgoing radiance and the scene stored in voxels.
<!--more-->

# Overview

Deferred voxel shading is a four-step real-time global illumination technique inspired by voxel cone tracing and deferred rendering. This approach enables us to obtain an accurate approximation of a plethora of indirect illumination effects including: indirect diffuse, specular reflectance, color-blending, emissive materials, indirect shadows and ambient occlusion. The steps that comprehend this technique are described below.

<table>
  <tr>
    <th>Technique Overview</th>
  </tr>
  <tr>
    <td align="center"><img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497489937/dvsgi_overview_vlucdk.svg" style="width: 100%;"/></td>
  </tr>
</table>

## 1. Voxelization

The first step is to voxelize the scene, my implemention voxelizes scene albedo, normal and emission to approximate emissive materials. For this conservative voxelization and atomic operations on 3D textures are used as described [here](https://www.seas.upenn.edu/~pcozzi/OpenGLInsights/OpenGLInsights-SparseVoxelization.pdf).

```c
...
layout(binding = 0, r32ui) uniform volatile coherent uimage3D voxelAlbedo;
layout(binding = 1, r32ui) uniform volatile coherent uimage3D voxelNormal;
layout(binding = 2, r32ui) uniform volatile coherent uimage3D voxelEmission;
...
// average normal per fragments sorrounding the voxel volume
imageAtomicRGBA8Avg(voxelNormal, position, normal);
// average albedo per fragments sorrounding the voxel volume
imageAtomicRGBA8Avg(voxelAlbedo, position, albedo);
// average emission per fragments sorrounding the voxel volume
imageAtomicRGBA8Avg(voxelEmission, position, emissive);
...
```

| Scene | Average Albedo | Average Normal | Average Emission |
|-------|----------------|----------------|------------------|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497323827/DVSGI/scene_culelk.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497323820/DVSGI/v_albedo_qtc4ov.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497323804/DVSGI/v_normal_ryzmrh.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497323828/DVSGI/v_emission_aibyaf.png" style="width: 100%;"/>

### 1.1. Voxel Structure

My voxel structure is inspired by deferred rendering, where a G-Buffer contains relevant data to later be used in a separate light pass. Normal, albedo and emission values are stored in voxels during the voxelization process, every attribute has its own 3D texture associated. This information is sufficient to calculate the diffuse reflectance and normal attenuation on a separate light pass where, instead of computing the lighting per pixel it is done per voxel. The structure can be extended to support a more complicated reflectance model but this may imply a higher memory consumption to store additional data.

Furthermore, another structure is used for the voxel cone tracing pass. The resulting values of the lighting computations per voxel are stored in another 3D texture which we will call *radiance volume*. To approximate the incrementing diameter of the cone, and its sampling volume, levels of detail of the voxelized scene are used. For anisotropic voxels six 3D textures at half resolution of the radiance volume are required, one per every axis direction positive and negative, the levels of details are stored within the mipmap levels of these textures which we will call *directional volumes*. 

The radiance volume represents the maximum level of detail for the voxelized scene, this texture is separated from the directional volumes. To bind these two structures, linear interpolation is used between samples of both structures when the mipmap level required for the diameter of the cone ranges between, the maximum level and the first filtered level of detail.

<table>
  <tr>
    <th>A visualization of the voxel structure</th>
  </tr>
  <tr>
    <td align="center"><img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497571454/DVSGI/voxel_structure_dsmsvc.svg" style="width: 100%;"/></td>
  </tr>
</table>

### 1.2. Dynamic Voxelization

For dynamic updates, the conservative voxelization of static and dynamic geometry are separated. While static voxelization happens only once, dynamic voxelization happens per frame or when needed. The voxelization for both types of geometry rely on the same process, hence a way to indicate which voxels are static is needed. In my approach I use a single value 3D texture to identify voxels as static, this texture will be called *flag volume*.

During static voxelization, after a voxel is generated a value is written to the *flag volume* indicating this position as static. In contrast, during the dynamic voxelization, before generating a voxel, a value is read from the *flag volume* at the writing position of the voxel, if the value indicates this position is marked as static then writing is discarded, leaving the static voxels untouched.

To constantly revoxelize the scene it is necessary to clear from the 3D textures the previous stored dynamic voxels. This is done before the dynamic voxelization using compute shaders. The *flag volume* is read to clear voxels under two conditions: if the voxel exists and if it’s dynamic.

## 2. Voxel Illumination

The second step is voxel illumination, for this we calculate the outgoing radiance per voxel using the data stored from the voxelization step. For this we only need the voxel normal, and its position which can be easily extracted by projecting the voxel 3D coordinates in world-space, then we can calculate the direct lighting per voxel. This is all done in a compute shader.

One of the advantages of this technique is that it's compatible with all standard light types like point, spot and directional lights, another is that we don't need shadow maps, though they can help greatly with precision specially for directional lights. Other techniques calculate the voxel radiance per light's shadow map, meaning that for every shadow-mapped light that wants to contribute to the voxel radiance the illumination step has to be repeated.

| Scene | Voxel Direct Lighting |
|:-----:|:---------------------:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497323827/DVSGI/scene_culelk.png" style="width: 100%"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497323829/DVSGI/v_direct_vrnajc.png" style="width: 100%"/>

### 2.1. Normal-Weighted Attenuation

A disvantage of this technique is the loss of precision averaging all the geometry normals within a voxel. The resulting averaged normal may end up pointing towards a non-convenient direction. This problem is notable when the normal vectors within the space of a voxel are uneven:

<center>

![](http://res.cloudinary.com/jose-villegas/image/upload/v1497413810/DVSGI/uneven_normals_n3klcb.svg)

</center>

To reduce this issue my proposal utilizes a normal-weighted attenuation, where first the normal attenuation is calculated per every face of the voxel as follows:

<a href="https://www.codecogs.com/eqnedit.php?latex=D_{x,y,z}&space;=&space;(\hat{i}\cdot\Psi,&space;\hat{j}\cdot\Psi,&space;\hat{k}\cdot\Psi)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?D_{x,y,z}&space;=&space;(\hat{i}\cdot\Psi,&space;\hat{j}\cdot\Psi,&space;\hat{k}\cdot\Psi)" title="D_{x,y,z} = (\hat{i}\cdot\Psi, \hat{j}\cdot\Psi, \hat{k}\cdot\Psi)" /></a>

Then three dominant faces are selected depending on the axes sign of the voxel averaged normal vector:

<a href="https://www.codecogs.com/eqnedit.php?latex=D_{\omega}&space;=&space;\begin{cases}&space;max\{D_{\omega},0\},&space;&&space;N_{\omega}&space;>&space;0&space;\\&space;max\{-D_{\omega},0\},&space;&&space;\text{otherwise}&space;\end{cases}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?D_{\omega}&space;=&space;\begin{cases}&space;max\{D_{\omega},0\},&space;&&space;N_{\omega}&space;>&space;0&space;\\&space;max\{-D_{\omega},0\},&space;&&space;\text{otherwise}&space;\end{cases}" title="D_{\omega} = \begin{cases} max\{D_{\omega},0\}, & N_{\omega} > 0 \\ max\{-D_{\omega},0\}, & \text{otherwise} \end{cases}" /></a>

And finally, the resulting attenuation is the product of every dominant face normal attenuation, multiplied with the weight per axis of the averaged normal vector of the voxel, the resulting reflectance model is computed as follows:

<a href="https://www.codecogs.com/eqnedit.php?latex=W&space;=&space;N^2\\&space;V_{r}&space;=&space;L_{i}\frac{\rho}{\pi}(W_x&space;D_x&space;&plus;&space;W_y&space;D_y&space;&plus;&space;W_z&space;D_z)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?W&space;=&space;N^2\\&space;V_{r}&space;=&space;L_{i}\frac{\rho}{\pi}(W_x&space;D_x&space;&plus;&space;W_y&space;D_y&space;&plus;&space;W_z&space;D_z)" title="W = N^2\\ V_{r} = L_{i}\frac{\rho}{\pi}(W_x D_x + W_y D_y + W_z D_z)" /></a>

where <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;L_i" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;L_i" title="L_i" /></a> is the light source intensity, <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\rho" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\rho" title="\rho" /></a> the voxel albedo, <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;N" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;N" title="N" /></a> the normal vector of the voxel and <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\Psi" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\Psi" title="\Psi" /></a> the light direction.

| Normal Attenuation | Normal-weighted Attenuation |
|--------------------|-----------------------------|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497414410/DVSGI/shading_standard_e7vzft.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497414410/DVSGI/shading_directional_uwyxxw.png" style="width: 100%;"/>

### 2.2. Voxel Occlusion

To generate accurate results during the cone tracing step the voxels need to be occluded, otherwise voxelized geometry that is supposed to have little to no outgoing radiance will contribute to the indirect lighting calculations.

The classic shadow mapping or alike techniques can be used to compute the voxels occlusion. The position of the voxel is projected in light space and the depth of the projected point is compared with the stored depth from the shadow map to determine if the voxel is occluded.

My proposal also computes occlusion using raycasting within a volume. Any of the resulting volumes from the voxelization process can be used since the algorithm only needs to determine if a voxel exists at a certain position. To determine occlusion of a voxel, a ray is traced from the position of voxel in the direction of the light, the volume is sampled to determine if at the position of the ray there is a voxel, if this condition is true then the voxel is occluded.

#### 2.2.1. Soft Voxel Shadows

Instead of stopping the ray as soon a voxel is found, soft shadows can be approximated with a single ray accumulating a value \\(\kappa\\) per collision and dividing by the traced distance <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;t" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;t" title="t" /></a>, i.e. <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\nu&space;=&space;\nu&space;&plus;&space;(1&space;-&space;\nu)\kappa\div&space;t" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\nu&space;=&space;\nu&space;&plus;&space;(1&space;-&space;\nu)\kappa\div&space;t" title="\nu = \nu + (1 - \nu)\kappa\div t" /></a>, where <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;1-\nu" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;1-\nu" title="1-\nu" /></a> represents the occlusion value after the accumulation is finished. This technique exploits the observation that, from the light point of view, the number of collisions will usually be higher for the rays that pass through the borders of the voxelized geometry.

| Hard Voxel Shadows | Soft Voxel Shadows |
|:------------------:|:------------------:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497500701/DVSGI/hard_voxel_shadows_ovj32i.svg" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497500702/DVSGI/soft_voxel_shadows_gcmlzm.svg" style="width: 100%;"/>
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497500851/DVSGI/hard_traced_takadd.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497500853/DVSGI/soft_traced_upabyg.png" style="width: 100%;"/>

### 2.3. Emission

Adding to the final radiance value the emission term can be used approximate emissive surfaces such as area lights, neon lights, digital screens, etc., this provides a crude approximation of the emission term the rendering equation. After the direct illumination value is obtained the emission term from the voxelization process is added to this value per voxel. During the cone tracing step, these voxels will appear to be bright even on occluded areas, hence indirect light is accumulated per cone from these regions of the voxelized scene.

## 3. Anisotropic Voxels

For more precise results during the cone tracing step anisotropic voxels are used. The mipmapping levels, as seen in the *directional volumes* in the voxel structure, will store per every voxel six directional values, one per every directional axis positive and negative. Each cone has an origin, aperture angle and direction, this last factor determines which three volumes are sampled. The directional sample is obtained by linearly interpolating the three samples obtained from the selected directional volumes.

To generate the anisotropic voxels I use the process detailed by Crassin [here](http://maverick.inria.fr/Publications/2011/CNSGE11b/). To compute a directional value a step of volumetric integration is done in depth and the directional values are averaged to obtain the resulting value for a certain direction. In my approach this is done using compute shaders, executing a thread per every voxel at the mipmap level that is going to be filtered using the values from the previous level, this process is done per every mipmap level.

<table>
  <tr>
    <th>Process to generate anisotropic voxels</th>
  </tr>
  <tr>
    <td align="center"><img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497576308/DVSGI/aniso_cropped_aekdhv.png" style="width: 100%;"/></td>
  </tr>
</table>

## 4. Voxel Cone Tracing

Voxel cone tracing is similar to ray marching, the cone advances a certain length every step, except that the sampling volume increases along the diameter of the cone. The mipmap levels in the directional volumes are used to approximate the expansion of the sampling volume during the cone trace, to ensure smooth variation between samples quadrilinear interpolation is used which is natively supported with graphics hardware for 3D textures. This step is done in the fragment shader as a screenspace effect, a deferred rendering backend is needed, though voxel cone tracing can be done in forward rendering regardless, it would be extremely inneficient and expensive.

The shape of the cone is meant to exploit the spatial and directional coherence of the many rays packed within the space of a cone. This behavior is used in many approaches such as packet ray-tracing.

<table>
  <tr>
    <th>Visual representation of a cone used for cone tracing</th>
  </tr>
  <tr>
    <td align="center"><img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497581558/DVSGI/cone_oyxusj.svg" style="width: 100%;"/></td>
  </tr>
</table>

As seen in the figure above each cone is defined by an origin <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;C_o" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;C_o" title="C_o" /></a>, a direction <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;C_d" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;C_d" title="C_d" /></a> and an aperture angle <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\theta" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\theta" title="\theta" /></a>. During the cone steps the diameter of the cone is defined by <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;d" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;d" title="d" /></a>, this value can be extracted using the traced distance<a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;t" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;t" title="t" /></a> with the following equation: <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;d=2t\times\tan(\theta\div&space;2)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;d=2t\times\tan(\theta\div&space;2)" title="d=2t\times\tan(\theta\div 2)" /></a>. Which mipmap level should be sampled depending on the diameter of the cone can be obtained using the following equation: <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;V_{level}&space;=&space;log_2(d\div&space;V_{size})" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;V_{level}&space;=&space;log_2(d\div&space;V_{size})" title="V_{level} = log_2(d\div V_{size})" /></a>, where <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;V_{size}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;V_{size}" title="V_{size}" /></a> is the size of a voxel at the maximum level of detail.

As described by Crassin [here](http://maverick.inria.fr/Publications/2011/CNSGE11b/), for each cone trace we keep track of the occlusion value <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\alpha" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\alpha" title="\alpha" /></a> and the color value <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;c" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;c" title="c" /></a> which represents the indirect light towards the cone origin <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;C_o" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;C_o" title="C_o" /></a>. In each step we retrieve from the voxel structure the occlusion value <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\alpha_2" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\alpha_2" title="\alpha_2" /></a> and the outgoing radiance <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;c_2" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;c_2" title="c_2" /></a>. Then the <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;c" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;c" title="c" /></a> and <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\alpha" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\alpha" title="\alpha" /></a> values are updated using volumetric front-to-back accumulation as follows: <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;c&space;=\alpha&space;c&space;&plus;&space;(1-\alpha)\alpha_2c_2" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;c&space;=\alpha&space;c&space;&plus;&space;(1-\alpha)\alpha_2c_2" title="c =\alpha c + (1-\alpha)\alpha_2c_2" /></a> and <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\alpha=\alpha&plus;(1-\alpha)\alpha_2" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\alpha=\alpha&plus;(1-\alpha)\alpha_2" title="\alpha=\alpha+(1-\alpha)\alpha_2" /></a>. 

To ensure good integration quality between samples the distance <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;d'" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;d'" title="d'" /></a> between steps is modified by a factor <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\beta" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\beta" title="\beta" /></a>. With <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\beta=1" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\beta=1" title="\beta=1" /></a> the value of <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;d'" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;d'" title="d'" /></a> is equivalent to the current diameter <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;d" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;d" title="d" /></a> of the cone, values less than *1* produce higher quality results but require more samples which reduces the performance.

### 4.1. Indirect Illumination

Indirect lighting is approximated with a crude Monte Carlo approximation. The hemisphere region for the integral in the rendering equation can be partitioned into a sum of integrals. For a regular partition, each partitioned region resembles a cone. For each cone their contribution is approximated using voxel cone tracing, the resulting values are then weighted to obtain the accumulated contribution at the cones origin.

The distribution of the cones matches the shape of the BRDF, for a Blinn-Phong material a few large cones distributed over the normal oriented hemisphere estimate the diffuse reflection, while a single cone in the reflected direction, where its aperture depends on the specular exponent, approximates the specular reflection.

| Diffuse Cones | Specular Cone | BRDF |
|:-------------:|:-------------:|:----:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497581804/DVSGI/diffuse_cones_oo7hsx.svg" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497581807/DVSGI/specular_cones_faycaz.svg" style="width: 100%;"/>|<img src="https://res.cloudinary.com/jose-villegas/image/upload/v1497581810/DVSGI/brdf_cones_b4hmdk.svg" style="width: 100%;"/>

### 4.2. Ambient Occlusion

Ambient occlusion can be approximated using the same cones used for the diffuse reflection for efficiency. For the ambient occlusion term <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\delta" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\delta" title="\delta" /></a> we only accumulate the occlusion value <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\alpha_2" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\alpha_2" title="\alpha_2" /></a>, at each step the accumulated value is multiplied with the weighting function <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;f(r)&space;=&space;\frac{1}{1&plus;\lambda&space;r}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;f(r)&space;=&space;\frac{1}{1&plus;\lambda&space;r}" title="f(r) = \frac{1}{1+\lambda r}" /></a>, where <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;r" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;r" title="r" /></a> is the current radius of the cone and <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\lambda" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\lambda" title="\lambda" /></a> an user defined value which controls how fast <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;f(r)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;f(r)" title="f(r)" /></a> decays along the traced distance. At each cone step the ambient occlusion term is updated as: <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\delta&space;=&space;\delta&space;&plus;&space;(1-\delta)\alpha_2f(r)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\delta&space;=&space;\delta&space;&plus;&space;(1-\delta)\alpha_2f(r)" title="\delta = \delta + (1-\delta)\alpha_2f(r)" /></a>.

### 4.3. Soft Shadows

Cone tracing can also be used to achieve soft shadows tracing a cone from the surface point *x* towards the direction of the light The cone aperture controls how soft and scattered the resulting shadow is. For soft shadows with cones we only accumulate the occlusion value <a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;\alpha_2" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;\alpha_2" title="\alpha_2" /></a> at each step.

<table>
  <tr>
    <th>Cone Soft Shadows</th>
  </tr>
  <tr>
    <td align="center"><img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497585029/DVSGI/cone_shadow_jwx9pd.svg" style="width: 100%;"/></td>
  </tr>
</table>

## Voxel Global Illumination
To calculate the diffuse reflection over a surface point using voxel cone tracing we need its normal vector, albedo and the incoming radiance at that point. Since we voxelize the geometry normal vectors and the albedo into 3D textures, all the needed information for the indirect diffuse term is available after calculating the voxel direct illumination. In my approach I perform voxel cone tracing per voxel using compute shaders to calculate the first bounce of indirect diffuse at each voxel. This step is done after the outgoing radiance values from the voxel direct illumination pass are anisotropically filtered. 

For each voxel we use its averaged normal vector to generate a set of cones around the normal oriented hemisphere to calculate the indirect diffuse at the position of the voxel. The weighted result from all the cones is then multiplied by the albedo of the voxel and added to the direct illumination value. The resulting outgoing radiance for the radiance volume now stores the direct illumination, and the first bounce of indirect diffuse. The anisotropic filtering process needs be repeated for the new values. This enables us to approximate the second bounce of indirect lighting during the final voxel cone tracing step per pixel. This can be extended to support multiple bounces.

# Results

The results here were tested on an AMD 380 R9 GPU on different scenes with increasing geometric complexity and dynamic objects added to the scene. The scenes used for testing are listed below:

| Name | Model | Vertices | Triangles |
|:----:|:-----:|:--------:|:---------:|
S1|[Cornell Box](http://graphics.cs.williams.edu/data/meshes.xml)|72|36
S2|[Sibenik Cathedral](http://graphics.cs.williams.edu/data/meshes.xml)|40.479|75.283
S3|[Crytek Sponza](http://www.crytek.com/cryengine/cryengine3/downloads)|153.635|278.163

## Voxelization

The times per frame for the dynamic and static voxelization of each scene are show below: 

| Name | Static | Clear Dynamic | Dynamic |
|:----:|:------:|:-------------:|:-------:|
S1|0.51 ms|0.78 ms|1.30 ms
S2|1.80 ms| 0.58 ms| 2.11 ms
S3|11.29 ms| 0.60 ms| 2.03 ms

For the voxelization process, a higher resolution for the voxel representation can actually be beneficial, because it reduces thread collisions for the writing operations, in the scene S3 there is a higher time for the static voxelization this reason, S3 has a higher triangle density per voxel meaning more syncing operations are needed.

## Voxel Illumination

The table below shows the results for voxel direct illumination with shadow mapping or raycasting for voxel occlusion and voxel global illumination, it also includes the time for anisotropic filtering after both steps. For voxel direct illumination the times are similar between all scenes using shadow mapping, while raycasting costs more performance it enables occlusion for any type of light source without shadow mapping.

| Name | Direct (with Shadow Mapping) | Direct (with Raycasting) | Anistropic Filtering | Global Illumination | Anistropic Filtering |
|:----:|:----------------------------:|:------------------------:|:--------------------:|:-------------------:|:--------------------:|
S1|1.33 ms|20.32 ms|1.39 ms|8.41 ms|1.38 ms
S2|0.95 ms|4.57 ms|1.38 ms|3.88 ms|1.37 ms
S3|1.13 ms|3.31 ms|1.37 ms|5.44 ms|1.38 ms

For raycasting, the amount of empty space in the scene affects how early the traced rays end, which affects the general performance. For high density scenes such as S3 most rays end early, in contrast the scene S1 takes a considerable amount of time using raycasting because the scene is mostly empty space, this same condition also applies for the voxel indirect diffuse step.

| Scene | Voxel Direct | Voxel Direct + Indirect Diffuse |
|:-----:|:------------:|:-------------------------------:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497742010/DVSGI/cornell_scene.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497740284/DVSGI/direct_voxel_cornell.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497740284/DVSGI/gi_voxel_cornell.png" style="width: 100%;"/>
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497742010/DVSGI/sibenik_scene.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497740284/DVSGI/voxel_direct_sibenik.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497740284/DVSGI/voxel_gi_sibenik.png" style="width: 100%;"/>
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497742010/DVSGI/sponza_scene.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497740284/DVSGI/voxel_direct_sponza.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497740284/DVSGI/voxel_gi_sponza.png" style="width: 100%;"/>

## Voxel Cone Tracing

Once the voxel structure is anisotropically filtered we can proceed with voxel cone tracing per pixel for the final composition of the image. In my approach six cones with an aperture of 60 degrees are used to approximate the indirect diffuse term, for the indirect specular in these test I utilize a specular cone with an aperture of 10 degrees. The next table shows the performance for indirect lighting on different scenes. For all the scenes my implementation achieves results over 30FPS (\\(< 33.3 ms\\)) under constant dynamic update, meaning objects and lights are changing and moving per frame. The dynamic update includes dynamic voxelization, voxel direct illumination, voxel global illumination and the necessary anistropic filtering steps. For a screen resolution of \\(1920x1080\\) pixels my implementation obtains an average framerate of \\(28.57 ms\\) for the scene S3, \\(27.02 ms\\) for S2 and \\(27.77 ms\\) for S1 under constant dynamic update, these results show that even with a high resolution the technique can achieve over 30FPS for all the scenes.

|Scene | Direct | Indirect (Diffuse + Specular) | Direct + Indirect | Dynamic Stress |
|:----:|:------:|:-----------------------------:|:-----------------:|:--------------:|
S1 | 1.03 ms | 7.27 ms | 7.67 ms | 17.81 ms |
S2 | 1.32 ms | 7.62 ms | 8.32 ms | 17.60 ms |
S3 | 1.34 ms | 7.81 ms | 8.41 ms | 16.97 ms |

### Optimizations

In my implementation the dynamic update can be set to update at a given number of frames or on demand, a simple optimization for example is to update each 5~3 frames, this can easily give results over 60FPS for most scenes with little to zero notable difference.

Since the lighting step is separate from the voxelization step, revoxelization is only truly needed when objects move within the scene. For changes that only involve lighting revoxelization is unnecesary.

One optimization to consider that wasn't implemented in my application is to separate the indirect lighting calculation from voxel cone tracing at a lower resolution. The higher the resolution the more surface points that need to be cone traced since each pixel represents a position in scene. A disvantage of doing this is that it creates some notable artifacts, though they can be somewhat reduced through clever use of screen-space techniques.

## Show Off

### Dynamic Update

|Cornell Box|Sibenik Cathedral|Crytek Sponza|
|:---------:|:---------------:|:-----------:|
[![](http://res.cloudinary.com/jose-villegas/video/upload/v1497754650/DVSGI/cornell.jpg)](http://res.cloudinary.com/jose-villegas/video/upload/v1497754650/DVSGI/cornell.webm)|[![](http://res.cloudinary.com/jose-villegas/video/upload/v1497754650/DVSGI/sibenik.jpg)](http://res.cloudinary.com/jose-villegas/video/upload/v1497754650/DVSGI/sibenik.webm)|[![](http://res.cloudinary.com/jose-villegas/video/upload/v1497754650/DVSGI/sponza.jpg)](http://res.cloudinary.com/jose-villegas/video/upload/v1497754650/DVSGI/sponza.webm)

(*These are videos*)

### Indirect Lighting

|Emissive Material|Indirect Shadows|
|:---------------:|:--------------:|
[![](http://res.cloudinary.com/jose-villegas/video/upload/v1497757150/DVSGI/emissive.jpg)](http://res.cloudinary.com/jose-villegas/video/upload/v1497757150/DVSGI/emissive.webm)|[![](http://res.cloudinary.com/jose-villegas/video/upload/v1497757746/DVSGI/indirect_shadows.jpg)](http://res.cloudinary.com/jose-villegas/video/upload/v1497757746/DVSGI/indirect_shadows.webm)

(*These are videos*)

### Ambient Occlusion

| Cornell Box | Sibenik Cathedral | Crytek Sponza |
|:-----------:|:-----------------:|:-------------:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758102/DVSGI/cornell_ao.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758116/DVSGI/sibenik_ao.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758109/DVSGI/sponza_ao.png" style="width: 100%;"/>

### Soft Shadows

| 1 degree aperture | 5 degree aperture | 10 degree aperture | 25 degree aperture |
|:------:|:------:|:------:|:------:|
<img src="https://res.cloudinary.com/jose-villegas/image/upload/v1497758213/DVSGI/shadow_1.png" style="width: 100%;"/>|<img src="https://res.cloudinary.com/jose-villegas/image/upload/v1497758213/DVSGI/shadow_5.png" style="width: 100%;"/>|<img src="https://res.cloudinary.com/jose-villegas/image/upload/v1497758213/DVSGI/shadow_10.png" style="width: 100%;"/>|<img src="https://res.cloudinary.com/jose-villegas/image/upload/v1497758213/DVSGI/shadow_25.png" style="width: 100%;"/>

### Comparison

| Reference (3~ hours) | [LPV](http://www.crytek.com/download/Light_Propagation_Volumes.pdf) (18.86 ms) | My approach (17.34 ms) |
|:------:|:------:|:------:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758681/DVSGI/reference.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758675/DVSGI/lpv_comp.png" style="width: 100%;"/>|<img src="https://res.cloudinary.com/jose-villegas/image/upload/v1497758664/DVSGI/sponza_comp.png" style="width: 100%;"/>

### Emissive Materials / Area Lights

| &nbsp; | &nbsp; |
|:------:|:------:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758681/DVSGI/area_sponza.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758675/DVSGI/area_shadows.png" style="width: 100%;"/>
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758681/DVSGI/fine_emissive.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758675/DVSGI/area_teapot.png" style="width: 100%;"/>

### Teasers

| &nbsp; | &nbsp; |
|:------:|:------:|
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758681/DVSGI/teaser.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758675/DVSGI/teaser2.png" style="width: 100%;"/>
<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758681/DVSGI/teaser3.png" style="width: 100%;"/>|<img src="http://res.cloudinary.com/jose-villegas/image/upload/v1497758675/DVSGI/teaser4.png" style="width: 100%;"/>

### Recording

[![](https://img.youtube.com/vi/HwGSoTyP-oM/0.jpg)](http://www.youtube.com/watch?v=HwGSoTyP-oM)
[![](https://img.youtube.com/vi/52nkpkVZt-g/0.jpg)](http://www.youtube.com/watch?v=52nkpkVZt-g)
[![](https://img.youtube.com/vi/e1r5VrDtG7k/0.jpg)](http://www.youtube.com/watch?v=e1r5VrDtG7k)

# Executable

The executable application for this technique can be found in this link [here](https://github.com/jose-villegas/VCTRenderer/releases)
