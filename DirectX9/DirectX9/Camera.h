#pragma once

#include "direct3D.h"

//カメラ操作を行うクラス
//カメラの座標と注視点をコントロールする
//ビュー行列を出力

class Camera
{
private:
	D3DXVECTOR3 eyePoint;
	D3DXVECTOR3 relLookAtPoint;//注視点　eyePointからの座標差分
							   //視点が移動しても注視点と視点の
							   //位置関係を変えないようにする

	D3DXVECTOR3 lookAtPoint;   //グローバル座標での注視点
							   //相対座標での操作を行うので
							   //eyePoint + relLookAtPoint;
							   //が入る

	D3DXVECTOR3 upVec;		   //グローバル空間　上方向

	float pitchMax;
	float pitchMin;

public:
	Camera();
	~Camera();

	//位置の移動 引数はカメラの座標移動量
	void Move(float x, float y, float z);

	//現在の向きを基準にした移動　正面や左右を移動した際　上下の移動が起きる可能性がある
	void MoveBasedRotation(float forward, float right, float up);

	//現在の向きを基準にした移動(平面的)　正面や左右に移動した際に上下の移動が起きない
	void MoveBasedRotationFlatten(float forward, float right, float up = 0);

	//回転		 yaw   Y軸を軸にした回転　　
	//           Pitch 仰角伏角　上下の角度　完全に水平は0
	void Rotate(float radYaw, float radPitch);

	//位置の指定　移動の差分でなく　
	//移動したいグローバル座標の位置を直接指定
	void SetPosition(float x, float y, float z);

	//回転の指定
	//回転の変化量でなく回転の値を直接指定
	void SetRotation(float radYaw, float radPitch);

	//現在のカメラの状態に合わせた
	//ビュー行列の出力
	D3DXMATRIXA16 CreateViewMatrix();
};
