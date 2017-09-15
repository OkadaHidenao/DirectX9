#pragma once

template <class T>
class Vector2
{
private:
	T x;
	T y;

public:

	Vector2()
	{

	}

	//値の設定関数
	void Set(T x_, T y_)
	{
		x = x_;
		y = y_;
	}

	//値の取得の関数
	T& X() { return x; }	
	T& Y() { return y; }

	//演算子のオーバーロード
	//足し引きのみ定義

	//末尾のconstは　このメンバ関数を実行することによって
	//クラス内のメンバ変数の値に変更が起きないことを保証する
	//逆に言うとこの関数内でメンバ変数の変更が禁止される
	//　クラスインスタンスがconst付きで宣言されているとき
	//　このconstがついたメンバ関数しか実行できなくなる
	//　これによってconstの性質のデータの定数性が保たれる
	const Vector2<T> operator+(const Vector2<T>& other)const
	{
		Vector2<T> tmp;//作業用の一時変数　足した結果を入れる

		//足し算する
		tmp.Set(x + other.x, y + other.y);
		return tmp;
	}

	const Vector2<T> operator - (const Vector2<T>& other)const
	{
		Vector2<T>tmp;
		tmp.Set(x - other.x, y - other.y);
		return tmp;
	}

	//加算代入
	const Vector2<T> operator += (const Vector2<T>& other)const
	{
		x += other.x;
		y += other.y;

		return *(this);
	}
	const Vector2<T> operator -= (const Vector2<T>& other)const
	{
		x -= other.x;
		y -= other.y;

		return *(this);
	}

	//等価判断
	const bool operator == (const Vector2<T> other)const
	{
		return (x == ohter.x&&y == other.y);
	}
};