#pragma once
class Obstacle
{
public:
	/// <summary>
	/// Ctor/Dtor
	/// </summary>
	Obstacle();
	~Obstacle();

	/// <summary>
	/// Properties
	/// </summary>
	/// <returns></returns>
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetXOrigin() const;
	unsigned int GetYOrigin() const;
	unsigned int GetDepth() const;

private:
	/// <summary>
	/// Private members
	/// </summary>
	unsigned int x_origin_, y_origin_;
	unsigned int width_, height_;
	unsigned int depth_;
};

