//TODO: Modify the folowing code: 

int GlobalParameter : SasGlobal 
< 
   int3 SasVersion = {1, 1, 0}; 
   string SasEffectDescription = "This is a sample effect"; 
   string SasEffectCompany = "cgdev.net"; 
>; 

float4x4 World      : WORLD <string SasBindAddress = "Sas.Skeleton.MeshToJointToWorld[0]";>; 
float4x4 View       : VIEW <string SasBindAddress = "Sas.Camera.WorldToView";>; 
float4x4 Projection : PROJECTION <string SasBindAddress = "Sas.Camera.Projection";>; 

bool bEmissiveColor = false; 
bool bLight = true; 
float3 lightDirection = { 0, 0, -1 }; 

float4 DiffuseColor  = { 1.000000f, 1.000000f, 1.000000f, 1.000000f }; 
float  SpecularPower = 10.000000f; 
float4 SpecularColor = { 0.000000f, 0.000000f, 0.000000f, 1.000000f }; 
float4 EmissiveColor = { 0.000000f, 0.000000f, 0.000000f, 1.000000f }; 

texture MI_AK-47; 
texture Map__1; 

sampler2D MI_AK-47Sampler = sampler_state  // TexCoord0 
{ 
   Texture = <MI_AK-47>; 
   MinFilter = Linear; 
   MagFilter = Linear; 
   MipFilter = Linear; 
   AddressU  = Wrap;     
   AddressV  = Wrap;     
}; 

sampler2D Map__1Sampler = sampler_state  // TexCoord0 
{ 
   Texture = <Map__1>; 
   MinFilter = Linear; 
   MagFilter = Linear; 
   MipFilter = Linear; 
   AddressU  = Wrap;     
   AddressV  = Wrap;     
}; 

struct VS_INPUT 
{ 
   float4 Position  : POSITION; 
   float2 TexCoord0 : TEXCOORD0;
   float3 N         : NORMAL;
   float3 T         : TANGENT;
   float3 B         : BINORMAL;
}; 

struct VS_OUTPUT 
{ 
   float4 Position  : POSITION; 
   float2 TexCoord0 : TEXCOORD0;
   float LightStrength     : COLOR1;
}; 

VS_OUTPUT VS( VS_INPUT IN ) 
{ 
   //TODO: Modify the folowing code: 
   VS_OUTPUT OUT;	 
   float4 oPos = mul( mul( IN.Position, World ), View ); 
   OUT.Position = mul( oPos, Projection ); 
   OUT.TexCoord0.xy = IN.TexCoord0.xy; 
   float3 WorldNormal = normalize( mul( IN.N, (float3x3)World ) );
   OUT.LightStrength =  max(0, dot( WorldNormal, lightDirection ));
   return OUT;
}; 

float4  PS( VS_OUTPUT vout ) : COLOR 
{ 
   //TODO: Modify the folowing code: 
   float4 color = tex2D( MI_AK-47Sampler, vout.TexCoord0 ); 
   color.rgb = color.rgb * vout.LightStrength; 
   if(bEmissiveColor)
     color.rgb += EmissiveColor.rgb;
   return  color; 
}; 

technique DefaultTechnique 
{ 
   pass p0 
   { 
      VertexShader = compile vs_3_0  VS(); 
      PixelShader  = compile ps_3_0  PS(); 
   } 
} 
