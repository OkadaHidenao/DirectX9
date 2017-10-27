#include "Camera.h"

Camera::Camera()
{
	//初期　原点
	eyePoint = D3DXVECTOR3(0, 0, 0);

	//グローバル座標での上方向
	upVec = D3DXVECTOR3(0, 1, 0);//y軸 プラス

								 //カメラ注視点　eyePointを中心にした相対座標
								 //初期値はzプラス方向 で水平を向くように設定
	relLookAtPoint = D3DXVECTOR3(0, 0, 1);

	//注視点　グローバル座標
	lookAtPoint = eyePoint + relLookAtPoint;

	//仰角俯角の限度を設定　初期値は+-36度
	pitchMax = 3.1415f*(1.0f / 5.0f);
	pitchMin = -3.1415f*(1.0f / 5.0f);
}


Camera::~Camera()
{
	//特にnew 等していないため空
}

//カメラの向きを考慮しない移動
//グローバル空間でのそれぞれの軸の変化量を指定
void Camera::Move(float x, float y, float z)
{
	//カメラ視点　移動量
	D3DXVECTOR3 delta(x, y, z);
	//視点移動
	eyePoint += delta;
}

//カメラの回転 (変化量指定)
// yaw   Y軸を軸にした回転　　
// Pitch 仰角伏角　上下の角度　
void Camera::Rotate(float radYaw, float radPitch)
{
	//まずは現在の YawとPitchの状態を調べる
	double x = relLookAtPoint.x;
	double y = relLookAtPoint.y;
	double z = relLookAtPoint.z;

	//z,x からyaw回転の角度を求める　(ラジアン)
	//atan2 アークタンジェント
	double first_radYaw = atan2(z, x);

	//pitch回転を求める

	//relLookAtPointからyの要素を取り除いたベクトルの長さ
	//三平方の定理で計算
	//sqrt SquareRootの略　平方根
	double pitchX = sqrt(x*x + z*z);
	//pitchの角度計算
	double first_radPitch = atan2(y, pitchX);

	//新しいrelLookAtの各要素を計算
	//まずx zから
	//三角関数の加法定理を利用

	//サインの加法定理を利用
	double resultZ = sin(first_radYaw)*cos(radYaw)
		+ cos(first_radYaw)*sin(radYaw);

	//コサインの加法定理を利用
	double resultX = cos(first_radYaw)* cos(radYaw)
		- sin(first_radYaw)*sin(radYaw);

	//ピッチ　伏角仰角を計算
	//角度に上限下限を設定
	double addRadPitch = radPitch;

	//180*(1/5) 36度　を超えていたら
	//プラス方向の制限
	if (first_radPitch + addRadPitch >= pitchMax)
	{
		addRadPitch = pitchMax - first_radPitch;
		//回転後に制限の角度と一致するまで
		//変化量を減らす		
	}
	//マイナス方向の制限
	if (addRadPitch + first_radPitch <= pitchMin)
	{
		addRadPitch = pitchMin - first_radPitch;
	}

	//新しいrelLookAtの各要素を計算
	//yを計算

	//サインの加法定理を利用
	double resultY = sin(first_radPitch)*cos(addRadPitch)
		+ cos(first_radPitch)*sin(addRadPitch);

	//resultX resultYのみのベクトルの長さ
	//コサインの加法定理を利用
	double resultPitchX = cos(first_radPitch)*cos(addRadPitch)
		- sin(first_radPitch)*sin(addRadPitch);

	//現在 resultX resultZ　合わせて長さが1なので
	//PitchXで縮小
	//(resultX resultY resultZ 
	//合わせて長さが1のベクトルになるようにする) 
	resultX *= resultPitchX;
	resultZ *= resultPitchX;

	relLookAtPoint = D3DXVECTOR3(
		static_cast<float>(resultX),
		static_cast<float>(resultY),
		static_cast<float>(resultZ));

	//ベクトルの標準化　念のために
	D3DXVec3Normalize(&relLookAtPoint, &relLookAtPoint);
}

//現在のメンバの値からビュー行列を作成し返す
D3DXMATRIXA16 Camera::CreateViewMatrix()
{
	//注視点　グローバル座標
	lookAtPoint = eyePoint + relLookAtPoint;

	D3DXMATRIXA16 view;

	//ビュー行列作成
	D3DXMatrixLookAtLH(&view, &eyePoint, &lookAtPoint, &upVec);

	return view;
}

void Camera::SetPosition(float x, float y, float z)
{
	eyePoint = D3DXVECTOR3(x, y, z);
}

void Camera::SetRotation(float yaw, float pitch)
{
	//ピッチの制限にかかっているかをチェック
	//プラス方向
	if (pitch > pitchMax)
	{
		pitch = pitchMax;
	}
	//マイナス方向
	if (pitch < pitchMin)
	{
		pitch = pitchMin;
	}

	//各軸の要素を計算
	double y = sin(pitch);

	//x zのみでベクトルを作った場合の長さ
	double pitchX = cos(pitch);

	//yawが0だとxのプラス方向を向く
	double x = cos(yaw)*pitchX;
	double z = sin(yaw)*pitchX;

	relLookAtPoint = D3DXVECTOR3(
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(z));
}

//現在の向きを基準にした移動　正面や左右を移動した際　上下の移動が起きる可能性がある
void Camera::MoveBasedRotation(float forward, float right, float up)
{
	D3DXVECTOR3 forwardVec = relLookAtPoint;

	//上方向と右方向を計算

	//まずは現在の YawとPitchの状態を調べる
	double x = relLookAtPoint.x;
	double y = relLookAtPoint.y;
	double z = relLookAtPoint.z;

	//z,x からyaw回転の角度を求める　(ラジアン)
	//atan2 アークタンジェント
	double first_radYaw = atan2(z, x);

	D3DXVECTOR3 rightVec = 
		D3DXVECTOR3(cos(first_radYaw - (3.1415f*0.5f)),//正面方向から90度分減産
					0,//ロール回転がないので左右でy軸の変化はなし
					sin(first_radYaw - (3.1415f*0.5f)));

	//上方向
	double pitchX = sqrt(x*x + z*z);
	double first_radPitch = atan2(y, pitchX);//現在のピッチの角度
	double cosPitch = cos(first_radPitch + 3.1415f*0.5f);//正面方向から90度分加算

	D3DXVECTOR3 upVector=
		D3DXVECTOR3(x*cosPitch,
					sin(first_radPitch + 3.1415f*0.5f),
					z*cosPitch);

	eyePoint += upVector*up + forwardVec*forward + rightVec*right;
}

//現在の向きを基準にした移動(平面的)　正面や左右に移動した際に上下の移動が起きない
//上下移動は固定でy軸の+-方向
void Camera::MoveBasedRotationFlatten(float forward, float right,float up)
{
	double x = relLookAtPoint.x;
	double y = relLookAtPoint.y;
	double z = relLookAtPoint.z;

	double first_radYaw = atan2(z, x);

	D3DXVECTOR3 forwardVec =
		D3DXVECTOR3(cos(first_radYaw),
					0,
					sin(first_radYaw));

	D3DXVECTOR3 rightVec =
		D3DXVECTOR3(cos(first_radYaw - (3.1415f*0.5f)),//正面方向から90度分減産で右方向
					0,
					sin(first_radYaw - (3.1415f*0.5f)));

	eyePoint += upVec*up + forwardVec*forward + rightVec*right;
}