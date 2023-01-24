#include "Skybox.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	//	�J�������烏�[���h���W�ւ̃x�N�g�����Z�o���A��������ʏ�̍��W�Ƃ��Ĉ���
	float3 E = normalize(pin.worldPosition.xyz - viewPosition.xyz);

	//	�����x�N�g����UV���W�֕ϊ�����
	float2	texcoord;
	{
		static const float PI = 3.141592654f;
		texcoord = float2(1.0f - atan2(E.z, E.x) / 2, -atan2(E.y, length(E.xz)));
		texcoord = texcoord / PI + 0.5f;
	}

	return texture0.Sample(sampler0, texcoord) * pin.color;
}