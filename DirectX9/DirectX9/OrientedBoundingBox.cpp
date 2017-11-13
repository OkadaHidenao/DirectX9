#include"OrientedBoundingBox.h"

#define FORWARD 0
#define RIGHT   1
#define UP      2

//F forward <-->B back
//R right   <-->L left
//U up      <-->D down
//頂点の場所をこれの組み合わせで表現

#define VERTEX_FRU 0 //正面　右　上
#define VERTEX_FRD 1 //正面　右　下
#define VERTEX_FLU 2 //正面　左　上
#define VERTEX_FLD 3 //正面　左　下
#define VERTEX_BRU 4 //後ろ　右　上
#define VERTEX_BRD 5 //後ろ　右　下
#define VERTEX_BLU 6 //後ろ　左　上
#define VERTEX_BLD 7 //後ろ　左　下

//コンストラクタ
OrientedBoundingBox::OrientedBoundingBox()
{

	//中心座標は原点で初期化
	position = D3DXVECTOR3(0, 0, 0);

	for (int i = 0; i < 3; i++)
	{
		//長さを全て１で初期化
		length[i] = 1;
	}

	//向きを初期化
	NormDirection[FORWARD] = D3DXVECTOR3(0, 0, 1);
	NormDirection[RIGHT]   = D3DXVECTOR3(1, 0, 0);
	NormDirection[UP]      = D3DXVECTOR3(0, 1, 0);

	//各頂点の更新
	UpdateVertexPos();
}

//デストラクタ
OrientedBoundingBox::~OrientedBoundingBox()
{
	
}

//新しい中心座標の設定
void OrientedBoundingBox::SetPosition(D3DXVECTOR3 newPos)
{
	position = newPos;

	//各頂点の更新
	UpdateVertexPos();
}
void OrientedBoundingBox::SetDirection(D3DXVECTOR3 newForward, D3DXVECTOR3 newRight, D3DXVECTOR3 newUp)
{
	//標準化して代入
	D3DXVec3Normalize(&newForward, &newForward);
	D3DXVec3Normalize(&newRight, &newRight);
	D3DXVec3Normalize(&newUp, &newUp);

	//それぞれの軸同士が直行するかの判断をしてから代入するのがよさそう

	//代入
	NormDirection[FORWARD] = newForward;
	NormDirection[RIGHT]   = newRight;
	NormDirection[UP]	   = newUp;
}

//各辺の長さを設定
void OrientedBoundingBox::SetLength(float forward, float right, float up)
{
	length[FORWARD] = forward;
	length[RIGHT]   = right;
	length[UP]      = up;

	//各頂点の更新
	UpdateVertexPos();
}

void OrientedBoundingBox::UpdateVertexPos()
{
	//前右上の頂点座標
	vertexPos[VERTEX_FRU]
		= position +
		(NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(NormDirection[RIGHT]   * length[RIGHT] * 0.5f) +
		(NormDirection[UP]	    * length[UP] * 0.5f);

	vertexPos[VERTEX_FRD]
		= position +
		(NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(NormDirection[RIGHT]   * length[RIGHT] * 0.5f) +
		(-NormDirection[UP]     * length[UP] * 0.5f);

	vertexPos[VERTEX_FLU]
		= position +
		(NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(-NormDirection[RIGHT]  * length[RIGHT] * 0.5f) +
		(NormDirection[UP]      * length[UP] * 0.5f);

	vertexPos[VERTEX_FLD]
		= position +
		(NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(-NormDirection[RIGHT]  * length[RIGHT] * 0.5f) +
		(-NormDirection[UP]     * length[UP] * 0.5f);


	//背面
	vertexPos[VERTEX_BRU]
		= position +
		(NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(NormDirection[RIGHT]   * length[RIGHT] * 0.5f) +
		(NormDirection[UP]      * length[UP] * 0.5f);

	vertexPos[VERTEX_BRD]
		= position +
		(NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(NormDirection[RIGHT]   * length[RIGHT] * 0.5f) +
		(-NormDirection[UP]     * length[UP] * 0.5f);

	vertexPos[VERTEX_BLU]
		= position +
		(-NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(-NormDirection[RIGHT]   * length[RIGHT] * 0.5f) +
		(NormDirection[UP]       * length[UP] * 0.5f);

	vertexPos[VERTEX_BLD]
		= position +
		(-NormDirection[FORWARD] * length[FORWARD] * 0.5f) +
		(-NormDirection[RIGHT]   * length[RIGHT] * 0.5f) +
		(-NormDirection[UP]      * length[UP] * 0.5f);
}

//分離軸に直方体を投影した長さを計算
static float LengthSegmentOnSepatateAxis(D3DXVECTOR3 *pSep,D3DXVECTOR3 *e1, D3DXVECTOR3 *e2,D3DXVECTOR3 *e3 = NULL)
{
	//pSep 分離軸　標準化ベクトル
	//e1～e3　それぞれの辺のベクトル

	//3つの内積の絶対値の合計が投影した線分長

	//fabs : floatabsolute
	//absolute 絶対値

	//D3DXVec3Dot
	//Dot : dot product 内積

	//内積　
	//標準化ベクトル同士 : 2つのベクトルのなす角のコサイン
	//通常のベクトル同士 : なす角のコサイン*ベクトル１の長さ*ベクトル2の長さ

	//r1はpSepとe1のなす角のコサイン*e1の長さ
	float r1 = fabs(D3DXVec3Dot(pSep, e1));
	float r2 = fabs(D3DXVec3Dot(pSep, e2));
	float r3 = (e3 != NULL) ? fabs(D3DXVec3Dot(pSep, e3)) : 0;

	//分離軸pSepに直方体を投影した長さ
	return r1 + r2 + r3;
}

bool OrientedBoundingBox::Collision(OrientedBoundingBox &obb_A, OrientedBoundingBox &obb_B)
{
	//2つのOBBが重なっているかを調べる
	//各辺のベクトルの1/2
	D3DXVECTOR3 AE[3];//box_A
	D3DXVECTOR3 BE[3];//box_B

	//長さと向きから計算
	for (int i = 0; i < 3; i++)
	{
		AE[i] = obb_A.NormDirection[i] * obb_A.length[i] * 0.5f;

		BE[i] = obb_B.NormDirection[i] * obb_B.length[i] * 0.5f;
	}

	//二つのOBBの中心同士を結ぶ直線のベクトル
	//BからAに向かう時のベクトル
	D3DXVECTOR3 Interval = obb_A.position - obb_B.position;

	//方向軸を分離軸にした際の分離のチェック
	float ra, rb, L;

	//obb_Aの方向軸

	//分離軸 obb_A.NormDirection[FORWARD]
	ra = obb_A.length[FORWARD] * 0.5f;
	//obb_Bは投影した長さを求める
	rb = LengthSegmentOnSepatateAxis
	(&obb_A.NormDirection[FORWARD], &BE[0], &BE[1], &BE[2]);
	//インターバルを投影した長さを求める
	L = fabs(D3DXVec3Dot(&obb_A.NormDirection[FORWARD], &Interval));
	if (L > ra + rb)
	{
		//分離平面が確認できたため　当たっていない
		return false;
	}

	//分離軸 obb_A.NormDirection[RIGHT]
	ra = obb_A.length[RIGHT] * 0.5f;
	rb = LengthSegmentOnSepatateAxis
	(&obb_A.NormDirection[RIGHT], &BE[0], &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&obb_A.NormDirection[RIGHT], &Interval));
	if (L > ra + rb)return false;

	//分離軸 obb_A.NormDirection[UP]
	ra = obb_A.length[UP] * 0.5f;
	rb = LengthSegmentOnSepatateAxis
	(&obb_A.NormDirection[UP], &BE[0], &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&obb_A.NormDirection[UP], &Interval));
	if (L > ra + rb)return false;




	//obb_Bの方向軸

	//分離軸 obb_B.NormDirection[FORWARD]
	ra = obb_B.length[FORWARD] * 0.5f;
	//obb_Aは投影した長さを求める
	rb = LengthSegmentOnSepatateAxis
	(&obb_B.NormDirection[FORWARD], &AE[0], &AE[1], &AE[2]);
	//インターバルを投影した長さを求める
	L = fabs(D3DXVec3Dot(&obb_B.NormDirection[FORWARD], &Interval));

	if (L > ra + rb)
	{
		//分離平面が確認できたため　当たっていない
		return false;
	}

	//分離軸 obb_B.NormDirection[RIGHT]
	ra = obb_B.length[RIGHT] * 0.5f;
	rb = LengthSegmentOnSepatateAxis
	(&obb_B.NormDirection[RIGHT], &AE[0], &AE[1], &AE[2]);
	L = fabs(D3DXVec3Dot(&obb_B.NormDirection[RIGHT], &Interval));
	if (L > ra + rb)return false;

	//分離軸 obb_B.NormDirection[UP]
	ra = obb_B.length[UP] * 0.5f;
	rb = LengthSegmentOnSepatateAxis
	(&obb_B.NormDirection[UP], &AE[0], &AE[1], &AE[2]);
	L = fabs(D3DXVec3Dot(&obb_B.NormDirection[UP], &Interval));
	if (L > ra + rb)return false;

	//各obbの方向軸では分離が見つからなかったので外積を分離軸にする
	//外積: cross
	//2つのベクトルにともに直交するベクトル

	//外積分離軸の略称
	//C00 : cross obb1.normdirection[0] obb2.normdirection[0]
	//C01 :                          0                     1

	D3DXVECTOR3 cross;

	//分離軸　C00
	//外積軸の算出
	D3DXVec3Cross(&cross, &obb_A.NormDirection[0], &obb_B.NormDirection[0]);
	//外積軸にobb_Aとobb_Bを投影した長さを計算
	//外積軸を求めるときに使用した方向の辺は投影に使用しない
	//投影したとしても点になるので長さ0とみなす
	ra = LengthSegmentOnSepatateAxis(&cross, &AE[1], &AE[2]);
	rb = LengthSegmentOnSepatateAxis(&cross, &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&cross, &Interval));
	if (L > ra + rb)return false;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i + j == 0)continue;
			//分離軸　cij
			//外積軸の算出
			D3DXVec3Cross(&cross,
				&obb_A.NormDirection[i], 
				&obb_B.NormDirection[j]);
			//外積軸にobb_Aとobb_Bを投影した長さを計算
			//外積軸を求めるときに使用した方向の辺は投影に使用しない
			//投影したとしても点になるので長さ0とみなす
			ra = LengthSegmentOnSepatateAxis(&cross, &AE[(i + 1) % 3], &AE[(i + 2) % 3]);
			rb = LengthSegmentOnSepatateAxis(&cross, &BE[(j + 1) % 3], &BE[(j + 2) % 3]);
			L = fabs(D3DXVec3Dot(&cross, &Interval));
			if (L > ra + rb)return false;
		}
	}
	//どの分離軸でチェックしていても
}


