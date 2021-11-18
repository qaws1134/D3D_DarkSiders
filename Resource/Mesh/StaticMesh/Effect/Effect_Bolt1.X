xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template VertexElement {
 <f752461c-1e23-48f6-b9f8-8350850f336f>
 DWORD Type;
 DWORD Method;
 DWORD Usage;
 DWORD UsageIndex;
}

template DeclData {
 <bf22e553-292c-4781-9fea-62bd554bdd93>
 DWORD nElements;
 array VertexElement Elements[nElements];
 DWORD nDWords;
 array DWORD data[nDWords];
}


Material Material__410 {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "VFX_gen_bolts.tga";
 }
}

Frame root {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame SM_VFX_BoltMesh6 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh SM_VFX_BoltMesh6 {
   8;
   89.773715;56.252640;-2.503964;,
   102.009574;0.492320;-5.226709;,
   134.917402;20.083448;1.537237;,
   122.121933;65.939398;4.078496;,
   85.998912;120.558720;3.974416;,
   62.775091;100.939296;0.351075;,
   41.146163;121.002496;0.082308;,
   52.473965;156.732006;-0.152333;;
   6;
   3;0,1,2;,
   3;3,0,2;,
   3;0,3,4;,
   3;5,0,4;,
   3;6,5,4;,
   3;7,6,4;;

   MeshMaterialList {
    1;
    6;
    0,
    0,
    0,
    0,
    0,
    0;
    { Material__410 }
   }

   DeclData {
    3;
    2;0;3;0;,
    1;0;5;0;,
    1;0;5;1;;
    56;
    3189415242,
    3184406175,
    1065074636,
    1064189952,
    1061175296,
    1065066496,
    1061027840,
    3189596930,
    3181874252,
    1065096949,
    1064222720,
    1065345024,
    1065222144,
    1065205760,
    3189953966,
    3182787011,
    1065072886,
    1062125568,
    1065353216,
    1050681344,
    1065222144,
    3190563789,
    3184946038,
    1065020916,
    1062100992,
    1061191680,
    1050378240,
    1061044224,
    3183662840,
    3179090239,
    1065244920,
    1062051840,
    1049862144,
    1049812992,
    1049534464,
    3181213778,
    3183285860,
    1065231674,
    1064140800,
    1049837568,
    1064779776,
    1049501696,
    3182507385,
    3154508702,
    1065289729,
    1063280640,
    1016520704,
    1060102144,
    1006632960,
    3182445770,
    1024101804,
    1065281322,
    1061191680,
    1016963072,
    1006632960,
    1007525888;
   }
  }
 }
}