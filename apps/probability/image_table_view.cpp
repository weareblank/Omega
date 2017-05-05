#include "image_table_view.h"
#include <assert.h>
#include "app.h"
#include "images/calcul1_icon.h"
#include "images/calcul2_icon.h"
#include "images/calcul3_icon.h"
#include "images/focused_calcul1_icon.h"
#include "images/focused_calcul2_icon.h"
#include "images/focused_calcul3_icon.h"

namespace Probability {

ImageCell::ImageCell() :
  HighlightCell()
{
}

void ImageCell::drawRect(KDContext * ctx, KDRect rect) const {
  KDCoordinate width = bounds().width();
  KDCoordinate height = bounds().height();
  ctx->fillRect(KDRect(0,0, width, k_imageMargin), KDColorWhite);
  ctx->fillRect(KDRect(0,0, k_imageMargin, height), KDColorWhite);
  ctx->fillRect(KDRect(0,height-k_imageMargin, width, k_imageMargin), KDColorWhite);
  ctx->fillRect(KDRect(width-k_imageMargin,0, k_imageMargin, height), KDColorWhite);
}

int ImageCell::numberOfSubviews() const {
  return 1;
}

View * ImageCell::subviewAtIndex(int index) {
  assert(index == 0);
  return &m_iconView;
}

void ImageCell::layoutSubviews() {
  KDCoordinate width = bounds().width();
  KDCoordinate height = bounds().height();
  m_iconView.setFrame(KDRect(k_imageMargin, k_imageMargin, width - 2*k_imageMargin, height-2*k_imageMargin));
}

void ImageCell::reloadCell() {
  HighlightCell::reloadCell();
  if (isHighlighted()) {
    m_iconView.setImage(m_focusedIcon);
  } else {
    m_iconView.setImage(m_icon);
  }
}

void ImageCell::setImage(const Image * image, const Image * focusedImage) {
  m_icon = image;
  m_focusedIcon = focusedImage;
}

ImageTableView::ImageTableView(Responder * parentResponder, Calculation * calculation, CalculationController * calculationController) :
  View(),
  Responder(parentResponder),
  m_selectableTableView(this, this, 0, 0, 0, 0, 0, 0, this, false, false),
  m_isSelected(false),
  m_selectedIcon(0),
  m_calculation(calculation),
  m_calculationController(calculationController)
{
}

void ImageTableView::setCalculation(Calculation * calculation, int index) {
  m_calculation = calculation;
  m_selectedIcon = index;
}

void ImageTableView::didBecomeFirstResponder() {
  m_selectableTableView.reloadData();
  m_isSelected = true;
  if (selectedRow() == -1) {
    selectCellAtLocation(0, 0);
  } else {
    selectCellAtLocation(selectedColumn(), selectedRow());
  }
  app()->setFirstResponder(&m_selectableTableView);
}

void ImageTableView::willExitResponderChain(Responder * nextFirstResponder) {
  m_calculationController->reload();
}

bool ImageTableView::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::OK) {
    m_calculationController->setCalculationAccordingToIndex(selectedRow());
    select(false);
    setHighlight(true);
    m_selectableTableView.reloadData();
    m_calculationController->reload();
    app()->setFirstResponder(parentResponder());
    return true;
  }
  return false;
}

void ImageTableView::select(bool select) {
  if (!select) {
    m_selectableTableView.deselectTable();
    m_isSelected = select;
    willDisplayCellForIndex(m_selectableTableView.cellAtLocation(0,0), 0);
  } else {
    m_isSelected = select;
    m_selectableTableView.selectCellAtLocation(0, m_selectedIcon);
  }
}

void ImageTableView::setHighlight(bool highlight) {
  if (highlight) {
    m_selectableTableView.selectCellAtLocation(0,0);
  } else {
    m_selectableTableView.deselectTable();
  }
}

int ImageTableView::numberOfRows() {
  if (m_isSelected) {
    return k_numberOfImages;
  }
  return 1;
}

HighlightCell * ImageTableView::reusableCell(int index) {
  assert(index >= 0);
  assert(index < k_numberOfImages);
  return &m_imageCells[index];
}

int ImageTableView::reusableCellCount() {
  return k_numberOfImages;
}

void ImageTableView::willDisplayCellForIndex(HighlightCell * cell, int index) {
  ImageCell * myCell = (ImageCell *)cell;
  const Image *  images[3] = {ImageStore::Calcul1Icon, ImageStore::Calcul2Icon, ImageStore::Calcul3Icon};
  const Image * focusedImages[3] = {ImageStore::FocusedCalcul1Icon, ImageStore::FocusedCalcul2Icon, ImageStore::FocusedCalcul3Icon};
  if (!m_isSelected) {
    myCell->setImage(images[m_selectedIcon], focusedImages[m_selectedIcon]);
  } else {
    myCell->setImage(images[index], focusedImages[index]);
  }
  myCell->reloadCell();
}

KDCoordinate ImageTableView::cellHeight() {
  return k_imageCellHeight;
}

int ImageTableView::numberOfSubviews() const {
  return 1;
}

View * ImageTableView::subviewAtIndex(int index) {
  assert(index == 0);
  return &m_selectableTableView;
}

void ImageTableView::layoutSubviews() {
  m_selectableTableView.setFrame(bounds());
}

}
