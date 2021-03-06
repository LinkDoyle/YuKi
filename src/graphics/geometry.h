﻿#pragma once

namespace yuki {
namespace graphic {
class Matrix3x2F {
 public:
  Matrix3x2F() : Matrix3x2F(0, 0, 0, 0, 0, 0) {}

  Matrix3x2F(float m11, float m12, float m21, float m22, float m31, float m32)
      : m11_(m11), m12_(m12), m21_(m21), m22_(m22), m31_(m31), m32_(m32) {}

  float m11() const { return m11_; }
  float m12() const { return m12_; }
  float m21() const { return m21_; }
  float m22() const { return m22_; }
  float m31() const { return m31_; }
  float m32() const { return m32_; }

 private:
  float m11_;
  float m12_;
  float m21_;
  float m22_;
  float m31_;
  float m32_;
};

class Transform2D : public Matrix3x2F {
 public:
  Transform2D() = default;

  Transform2D(float m11, float m12, float m21, float m22, float m31, float m32)
      : Matrix3x2F(m11, m12, m21, m22, m31, m32) {}

  static Transform2D identity() { return {1, 0, 0, 1, 0, 0}; }

  static Transform2D translation(float dx, float dy) {
    return {1, 0, 0, 1, dx, dy};
  }

  static Transform2D rotation(float theta);
  static Transform2D rotation(float x, float y, float theta);
  static Transform2D skew(float px, float py, float theta, float phi);
};

template <typename T>
class TSize {
 public:
  TSize() : width_(0), height_(0) {}

  template <typename U>
  TSize(const TSize<U>& other)
      : width_(other.width()), height_(other.height()) {}
  TSize(T width, T height) : width_(width), height_(height) {}

  constexpr T width() const { return width_; }
  constexpr T height() const { return height_; }
  constexpr void setWidth(T width) { width_ = width; }
  constexpr void setHeight(T height) { height_ = height; }

 private:
  T width_;
  T height_;
};

using Size = TSize<int>;
using SizeF = TSize<float>;

template <typename T>
class TPoint {
 public:
  TPoint(T x, T y) : x_(x), y_(y) {}

  constexpr T x() const { return x_; }
  constexpr T y() const { return y_; }
  constexpr void setX(T x) { x_ = x; }
  constexpr void setY(T y) { y_ = y; }

  TPoint<T>& operator+=(const TPoint<T>& other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
  }

  TPoint<T>& operator-=(const TPoint<T>& other) {
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
  }

 private:
  T x_;
  T y_;
};

using Point = TPoint<int>;
using PointF = TPoint<float>;

template <typename T>
class TLine {
 public:
  TLine() = default;

  TLine(const TPoint<T>& p1, const TPoint<T>& p2) : p1_(p1), p2_(p2) {}

  TLine(T x1, T y1, T x2, T y2) : p1_(x1, y1), p2_(x2, y2) {}

  constexpr T x1() const { return p1_.x(); }
  constexpr T y1() const { return p1_.y(); }
  constexpr void setX1(T x1) { p1_.setX(x1); }
  constexpr void setY1(T y1) { p1_.setY(y1); }

  constexpr T x2() const { return p2_.x(); }
  constexpr T y2() const { return p2_.y(); }
  constexpr void setX2(T x2) { p2_.setX(x2); }
  constexpr void setY2(T y2) { p2_.setY(y2); }

  constexpr const TPoint<T>& p1() const { return p1_; }
  constexpr const TPoint<T>& p2() const { return p2_; }
  constexpr void setP1(const TPoint<T>& p1) { p1_ = p1; }
  constexpr void setP2(const TPoint<T>& p2) { p1_ = p2; }

  void translate(const TPoint<T>& p) {
    p1_ += p;
    p2_ += p;
  }

  TLine<T> translated(const TPoint<T>& p) const { return {p1_ + p, p2_ + p}; }

 private:
  TPoint<T> p1_;
  TPoint<T> p2_;
};

using Line = TLine<int>;
using LineF = TLine<float>;

/**
 * \brief The TRect<T> class defines a rectangle in the plane using T precision
 * \tparam T the type of precision
 */
template <typename T>
class TRect {
 public:
  TRect() : TRect(0, 0, 0, 0) {}

  explicit TRect(const TSize<T>& size)
      : TRect(0, 0, size.width(), size.height()) {}

  template <typename U>
  TRect(const TRect<U>& other)
      : left_(other.left_),
        top_(other.top_),
        right_(other.right_),
        bottom_(other.bottom_) {}

  /**
   * \brief Constructs a rectangle with (left, top, right, bottom).
   */
  constexpr TRect(T left, T top, T right, T bottom)
      : left_(left), top_(top), right_(right), bottom_(bottom) {}

  /**
   * \brief Adds dx1, dy1, dx2 and dy2 respectively
   *        to the existing coordinates of the rectangle.
   */
  constexpr void adjust(T dx1, T dy1, T dx2, T dy2) {
    left_ += dx1;
    top_ += dy1;
    right_ += dx2;
    bottom_ += dy2;
  }

  /**
   * \brief Returns a new rectangle with dx1, dy1, dx2 and dy2 added
   * respectively to the existing coordinates of this rectangle.
   */
  constexpr TRect adjusted(T dx1, T dy1, T dx2, T dy2) const {
    return {left_ + dx1, top_ + dy1, right_ + dx2, bottom_ + dy2};
  }

  constexpr T right() const noexcept { return right_; }
  constexpr T bottom() const noexcept { return bottom_; }
  constexpr T left() const noexcept { return left_; }
  constexpr T top() const noexcept { return top_; }
  void setRight(T right) noexcept { right_ = right; }
  void setBottom(T bottom) noexcept { bottom_ = bottom; }
  void setLeft(T left) noexcept { left_ = left; }
  void setTop(T top) noexcept { top_ = top; }

  T width() const noexcept { return right_ - left_; }
  T height() const noexcept { return bottom_ - top_; }
  TSize<T> size() const noexcept { return {width(), height()}; };
  void setWidth(T value) noexcept { right_ = left_ + value; }
  void setHeight(T value) noexcept { bottom_ = top_ + value; }

  void setSize(const TSize<T>& size) noexcept {
    setWidth(size.width());
    setHeight(size.height());
  }

  void translate(T dx, T dy) noexcept {
    left_ += dx;
    top_ += dy;
    right_ += dx;
    bottom_ += dy;
  }

  void translate(const TPoint<T>& offset) noexcept {
    translate(offset.x(), offset.y());
  }

  TRect translated(T dx, T dy) noexcept {
    return {left_ + dx, top_ + dy, right_ + dx, bottom_ + dy};
  }

  TRect translated(const TPoint<T>& offset) noexcept {
    return translated(offset.x(), offset.y());
  }

 private:
  T left_;
  T top_;
  T right_;
  T bottom_;
};

using Rect = TRect<int>;
using RectF = TRect<float>;

template <typename T>
class TRoundedRect : public TRect<T> {
 public:
  constexpr TRoundedRect(const TRect<T>& rect, T radiusX, T radiusY)
      : TRect<T>(rect), radiusX_(radiusX), radiusY_(radiusY) {}

  constexpr TRoundedRect(T left, T top, T right, T bottom, T radiusX, T radiusY)
      : TRect<T>(left, top, right, bottom),
        radiusX_(radiusX),
        radiusY_(radiusY) {}

  constexpr T radiusX() const { return radiusX_; }
  constexpr T radiusY() const { return radiusY_; }
  void setRadiusX(T radiusX) { radiusX_ = radiusX; }
  void setRadiusY(T radiusY) { radiusY_ = radiusY; }

 private:
  T radiusX_;
  T radiusY_;
};

using RoundedRect = TRoundedRect<int>;
using RoundedRectF = TRoundedRect<float>;

template <typename T>
class TEllipse {
 public:
  TEllipse(T x, T y, T radiusX, T radiusY)
      : center_(x, y), radiusX_(radiusX), radiusY_(radiusY) {}

  TEllipse(const TPoint<T>& center, T radiusX, T radiusY)
      : center_(center), radiusX_(radiusX), radiusY_(radiusY) {}

  constexpr const TPoint<T>& center() const { return center_; }
  constexpr T x() const { return center_.x(); }
  constexpr T y() const { return center_.y(); }
  void setX(T x) { center_.setX(x); }
  void setY(T y) { center_.setY(y); }
  constexpr T radiusX() const { return radiusX_; }
  constexpr T radiusY() const { return radiusY_; }

 private:
  TPoint<T> center_;
  T radiusX_;
  T radiusY_;
};

using Ellipse = TEllipse<int>;
using EllipseF = TEllipse<float>;

template <typename T>
class TCircle {
 public:
  TCircle(T x, T y, T radius) : center_(x, y), radius_(radius) {}

  TCircle(const TPoint<T>& center, T radius)
      : center_(center), radius_(radius) {}

  constexpr const TPoint<T>& center() const { return center_; }
  constexpr T x() const { return center_.x(); }
  constexpr T y() const { return center_.y(); }
  void setX(T x) { center_.setX(x); }
  void setY(T y) { center_.setY(y); }
  constexpr T radius() const { return radius_; }

 private:
  TPoint<T> center_;
  T radius_;
};

using Circle = TCircle<int>;
using CircleF = TCircle<float>;
}  // namespace graphic
}  // namespace yuki
