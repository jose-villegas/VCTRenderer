/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "VCT Engine", "index.html", [
    [ "Deferred Voxel Shading for Real Time Global Illumination", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html", [
      [ "Table of Contents", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md1", null ],
      [ "Overview", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md2", [
        [ "1. Voxelization", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md3", [
          [ "1.1. Voxel Structure", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md4", null ],
          [ "1.2. Dynamic Voxelization", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md5", null ]
        ] ],
        [ "2. Voxel Illumination", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md6", [
          [ "2.1. Normal-Weighted Attenuation", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md7", null ],
          [ "2.2. Voxel Occlusion", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md8", [
            [ "2.2.1. Soft Voxel Shadows", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md9", null ]
          ] ],
          [ "2.3. Emission", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md10", null ]
        ] ],
        [ "3. Anisotropic Voxels", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md11", null ],
        [ "4. Voxel Cone Tracing", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md12", [
          [ "4.1. Indirect Illumination", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md13", null ],
          [ "4.2. Ambient Occlusion", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md14", null ],
          [ "4.3. Soft Shadows", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md15", null ]
        ] ],
        [ "Voxel Global Illumination", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md16", null ]
      ] ],
      [ "Results", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md17", [
        [ "Voxelization", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md18", null ],
        [ "Voxel Illumination", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md19", null ],
        [ "Voxel Cone Tracing", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md20", null ],
        [ "Comparison", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md21", [
          [ "Optimizations", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md22", null ]
        ] ],
        [ "Show Off", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md23", [
          [ "Dynamic Update", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md24", null ],
          [ "Indirect Lighting", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md25", null ],
          [ "Ambient Occlusion", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md26", null ],
          [ "Soft Shadows", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md27", null ],
          [ "Emissive Materials / Area Lights", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md28", null ],
          [ "Teasers", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md29", null ],
          [ "Recording", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md30", null ]
        ] ]
      ] ],
      [ "Executable", "md__c_1__users__jose__documents__projects__v_c_t_renderer__r_e_a_d_m_e.html#autotoc_md31", null ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Enumerator", "functions_eval.html", null ],
        [ "Related Functions", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"class_instance_pool.html#a7f9e728c17965c9fbca3cc27dcc41a6e",
"class_raw_format.html#a8f3c3e2197c81e215db7531447878776",
"class_transform.html#ad86a4bfb67227e767987fa0a6d67c6e8",
"functions_k.html",
"shadow__map__renderer_8cpp.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';