#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);	// 法線マップ
Texture2D shadowMap : register(t2);	// シャドウマップ

SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 diffuseColor =
	diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;


	float3 normal =
		normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;
	float3x3 CM =
	{ normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };

	float3 N = normalize(mul(normal, CM));
	float3 L = normalize(directionalLightData.direction.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	// マテリアル定数
	float3 ka = float3(1, 1, 1);
	float3 kd = float3(1, 1, 1);
	float3 ks = float3(1, 1, 1);
	float shiness = 128;

	// 環境光の計算
	float4 ambientLightColor2 = float4(0.3, 0.3, 0.3,1);

	float3 ambient = ka * ambientLightColor;
	// 環境光を暗くする
	ambient = 0.01f;

	// 平行光源のライティング計算
	float3 directionalDiffuse =
		CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
	float3 directionalSpecular =
		CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

	// 平行光源の影なので、平行光源に対して影を適応
	float3 shadow = CalcShadowColorPCFFilter(
		shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
	directionalDiffuse *= shadow;
	directionalSpecular *= shadow;



	// 点光源の処理
	float3 pointDiffuse = (float3)0;
	float3 pointSpecular = (float3)0;
	int i;
	for (i = 0; i < pointLightCount; ++i)
	{
		// ライトベクトルを算出
		float3 lightVector = pin.world_position - pointLightData[i].position.xyz;

		// ライトベクトルの長さを算出
		float lightLength = length(lightVector);

		// ライトの影響範囲外なら後の計算をしない。
		if (lightLength >= pointLightData[i].range)
			continue;

		// 距離減衰を算出する
		float attenuate = saturate(1.0f - lightLength / pointLightData[i].range);
		// 明るくする
		attenuate = 1.0;
		lightVector = lightVector / lightLength;
		pointDiffuse += CalcLambertDiffuse(N, lightVector,
			pointLightData[i].color.rgb, kd.rgb) * attenuate;
		pointSpecular += CalcPhongSpecular(N, lightVector,
			pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
	}

	// スポットライトの処理
	float3 spotDiffuse = (float3)0;
	float3 spotSpecular = (float3)0;
	for (i = 0; i < spotLightCount; ++i)
	{
		// ライトベクトルを算出
		float3 lightVector = pin.world_position - spotLightData[i].position.xyz;

		// ライトベクトルの長さを算出
		float lightLength = length(lightVector);

		if (lightLength >= spotLightData[i].range)
			continue;

		// 距離減衰を算出する
		float attenuate = saturate(1.0f - lightLength / spotLightData[i].range);

		lightVector = normalize(lightVector);

		// 角度減衰を算出してattenuateに乗算する
		float3 spotDirection = normalize(spotLightData[i].direction.xyz);
		float angle = dot(spotDirection, lightVector);
		float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
		attenuate = 2;
		attenuate *= saturate(1.0f - (spotLightData[i].innerCorn - angle) / area);
		spotDiffuse += CalcLambertDiffuse(N, lightVector,
			spotLightData[i].color.rgb, kd.rgb) * attenuate;
		spotSpecular += CalcPhongSpecular(N, lightVector,
			spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
	}

	float4 color = float4(ambient, diffuseColor.a);

	color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
	color.rgb += diffuseColor.rgb;
	color.rgb += directionalSpecular + pointSpecular + spotSpecular;
	//	TODO リムライティング　真っ白になる
	color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);
	//if (shadow.x <= 2.0f && shadow.y <= 2.0f && shadow.z <= 2.0f)
	//{
	//	shadow.x = 1.0f;
	//	shadow.y = 1.0f;
	//	shadow.z = 1.0f;
	//}
	color *= float4(shadow,1.0);

	return color;
}