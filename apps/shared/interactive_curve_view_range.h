#ifndef SHARED_INTERACTIVE_CURVE_VIEW_RANGE_H
#define SHARED_INTERACTIVE_CURVE_VIEW_RANGE_H

#include <stdint.h>
#include "memoized_curve_view_range.h"
#include "curve_view_cursor.h"
#include "interactive_curve_view_range_delegate.h"

namespace Shared {

class InteractiveCurveViewRange : public MemoizedCurveViewRange {
public:
  InteractiveCurveViewRange(CurveViewCursor * cursor, InteractiveCurveViewRangeDelegate * delegate);
  void setCursor(CurveViewCursor * cursor);
  uint32_t rangeChecksum() override;

  //CurveViewWindow
  bool yAuto();
  void setXMin(float f) override;
  void setXMax(float f) override;
  void setYMin(float f) override;
  void setYMax(float f) override;
  void setYAuto(bool yAuto);

  // Window
  void zoom(float ratio);
  void panWithVector(float x, float y);
  virtual void roundAbscissa();
  virtual void normalize();
  virtual void setTrigonometric();
  virtual void setDefault();
  void centerAxisAround(Axis axis, float position);
  void panToMakePointVisible(float x, float y, float topMarginRatio, float rightMarginRatio, float bottomMarginRation, float leftMarginRation);
protected:
  bool m_yAuto;
  InteractiveCurveViewRangeDelegate * m_delegate;
private:
  constexpr static float k_minFloat = 1E-8f;
  CurveViewCursor * m_cursor;
};

typedef void (InteractiveCurveViewRange::*ParameterSetterPointer)(float);
typedef float (InteractiveCurveViewRange::*ParameterGetterPointer)();
typedef void (InteractiveCurveViewRange::*RangeMethodPointer)();

}

#endif
