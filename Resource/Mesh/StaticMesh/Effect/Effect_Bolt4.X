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


Material Material__399 {
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

 Frame SM_VFX_BoltMesh5 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh SM_VFX_BoltMesh5 {
   8;
   56.257677;128.365107;-2.337118;,
   124.069235;93.921677;-0.894889;,
   74.612243;184.007526;-4.836822;,
   164.311603;138.988058;-2.552171;,
   181.768230;40.378362;-6.241530;,
   231.497677;64.415974;-7.633002;,
   206.194304;-12.769582;-8.129005;,
   250.707354;-4.755608;-9.524077;;
   6;
   3;0,1,2;,
   3;1,3,2;,
   3;1,4,3;,
   3;4,5,3;,
   3;4,6,5;,
   3;6,7,5;;

   MeshMaterialList {
    1;
    6;
    0,
    0,
    0,
    0,
    0,
    0;
    { Material__399 }
   }

   DeclData {
    3;
    2;0;3;0;,
    1;0;5;0;,
    1;0;5;1;;
    56;
    984472501,
    1026951497,
    1065336624,
    1061863424,
    1064722432,
    1006632960,
    1065222144,
    1023094817,
    1009984571,
    1065344324,
    1061863424,
    1060536320,
    1006632960,
    1060773888,
    3131619651,
    1026567054,
    1065337677,
    1064591360,
    1064722432,
    1065222144,
    1065222144,
    1022139917,
    1000970362,
    1065345991,
    1064591360,
    1060536320,
    1065222144,
    1060773888,
    1027054292,
    3166939736,
    1065331561,
    1061863424,
    1049731072,
    1006632960,
    1049272320,
    1025493249,
    3166891973,
    1065335691,
    1064591360,
    1049731072,
    1065222144,
    1049272320,
    1024549304,
    3163810729,
    1065339908,
    1061863424,
    1025097728,
    1006632960,
    1006632960,
    1024285277,
    3163637758,
    1065340583,
    1064591360,
    1025097728,
    1065222144,
    1006632960;
   }
  }
 }
}