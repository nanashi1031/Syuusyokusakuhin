#include "Skybox.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	//	カメラからワールド座標へのベクトルを算出し、それを球面上の座標として扱う
	float3 E = normalize(pin.worldPosition.xyz - viewPosition.xyz);

	//	視線ベクトルをUV座標へ変換する
	float2	texcoord;
	{
		static const float PI = 3.141592654f;
		texcoord = float2(1.0f - atan2(E.z, E.x) / 2, -atan2(E.y, length(E.xz)));
		texcoord = texcoord / PI + 0.5f;
	}

	return texture0.Sample(sampler0, texcoord) * pin.color;
}