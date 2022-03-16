/*
  ==============================================================================

    UiSlider.cpp
    Created: 28 May 2014 9:47:39am
    Author:  monotomy

  ==============================================================================
*/

#include "_H_UiElements.h"
#include "Controller.h"
#include "AppStyles.h"

UISlider::UISlider()
    : slider(new SliderWrapper(this)), is_mouse_down(false), _slider_fill_color(0x00000000),
      _slider_outline_color(0x00000000), _slider_knob_color(0x00000000), _last_painted_value(-999)
{
    setRepaintsOnMouseActivity(false);
    setInterceptsMouseClicks(false, true);
    setPaintingIsUnclipped(true);

    addAndMakeVisible(slider);
    slider->addListener(this);
    // slider->setVelocityBasedMode( true );
}

UISlider::~UISlider() { delete slider; }

void SliderWrapper::mouseDrag(const MouseEvent &e_)
{
    if (isTimerRunning() &&
        getTimerInterval() == GLOBAL_VALUE_HOLDER::getInstance()->LONG_MOUSE_DOWN_INTERVAL)
    {
        if (e_.getDistanceFromDragStartX() >
                getHeight() * GLOBAL_VALUE_HOLDER::getInstance()->MULTIDRAG_SENSITIVITY ||
            e_.getDistanceFromDragStartY() >
                getHeight() * GLOBAL_VALUE_HOLDER::getInstance()->MULTIDRAG_SENSITIVITY ||
            e_.getDistanceFromDragStartX() <
                (getHeight() * GLOBAL_VALUE_HOLDER::getInstance()->MULTIDRAG_SENSITIVITY) * -1 ||
            e_.getDistanceFromDragStartY() <
                (getHeight() * GLOBAL_VALUE_HOLDER::getInstance()->MULTIDRAG_SENSITIVITY) * -1)
        {
            stopTimer();
            Slider::mouseDrag(e_);
        }
    }
    else if (is_a_long_mouse_down_event)
        _owner->on_mouse_drag(e_);
    else
        Slider::mouseDrag(e_);
}

void SliderWrapper::mouseDown(const MouseEvent &e_)
{
    bool escape = false;
    if (_owner->_controller)
        escape = _owner->_controller->on_pre_mouse_down_escape(e_);

    if (escape)
        return;

    _owner->on_mouse_down(e_);
    Slider::mouseDown(e_);

    if (GLOBAL_VALUE_HOLDER::getInstance()->MULTIDRAG_ENABLE)
    {
        if (e_.mods == ModifierKeys::rightButtonModifier &&
            GLOBAL_VALUE_HOLDER::getInstance()->MULTIDRAG_AT_RIGHT_MOUSE)
            startTimer(1);
        else if (!GLOBAL_VALUE_HOLDER::getInstance()->MULTIDRAG_AT_RIGHT_MOUSE &&
                 _owner->use_long_mouse_down())
            startTimer(GLOBAL_VALUE_HOLDER::getInstance()->LONG_MOUSE_DOWN_INTERVAL);
    }
}

void SliderWrapper::timerCallback()
{
    if (getTimerInterval() == GLOBAL_VALUE_HOLDER::getInstance()->LONG_MOUSE_DOWN_INTERVAL ||
        getTimerInterval() == 1)
    {
        is_a_long_mouse_down_event = true;
        _owner->on_long_mouse_down();
    }
    else
        _owner->on_mouse_up(*mouse_up_fake);

    stopTimer();
}

void SliderWrapper::mouseUp(const MouseEvent &e_)
{
    is_a_long_mouse_down_event = false;
    stopTimer();
    _owner->on_mouse_up(e_);
    Slider::mouseUp(e_);
}

void SliderWrapper::mouseWheelMove(const MouseEvent &e_, const MouseWheelDetails &w_)
{
    if (!GLOBAL_VALUE_HOLDER::getInstance()->ENABLE_MOUSEWHEEL)
        return;

    _owner->on_mouse_down(e_);
    Slider::mouseWheelMove(e_, w_);
    startTimer(1000);
}

SliderWrapper::SliderWrapper(UISlider *const owner_)
    : _owner(owner_), is_a_long_mouse_down_event(false), mouse_up_fake(nullptr)
{
    setRepaintsOnMouseActivity(false);
    setBufferedToImage(false);
    setPaintingIsUnclipped(true);
}

bool UISlider::refresh_foreground()
{
    if (!_model || !_controller)
        return false;

    AppStyle *const style = _model->_style;
    String new_value_as_label = HAS_NO_TEXT_VALUE;

    bool should_be_repainted = false;

    uint32 _new_slider_fill_color = style->get_state_on_1_color();
    if (_slider_fill_color != _new_slider_fill_color)
    {
        _slider_fill_color = _new_slider_fill_color;
        should_be_repainted = true;
    }

    uint32 _new_slider_outline_color = style->get_slider_outline_color();
    if (_slider_outline_color != _new_slider_outline_color)
    {
        _slider_outline_color = _new_slider_outline_color;
        should_be_repainted = true;
    }

    uint32 _new_slider_knob_color = style->get_slider_knob_color();
    if (_slider_knob_color != _new_slider_knob_color)
    {
        _slider_knob_color = _new_slider_knob_color;
        should_be_repainted = true;
    }

    int current_value = _controller->get_value();
    if (_last_painted_value != current_value)
    {
        _last_painted_value = current_value;
        slider->setValue(current_value, dontSendNotification);
        should_be_repainted = true;
    }

    return should_be_repainted;
}

void SliderWrapper::cache_paint(Graphics &g_, uint32 background_colour_)
{
    MONO_Controller *controller = _owner->_controller;
    if (!controller)
        return;

    if (!_owner->_model->isVisible())
        return;

    int width = getWidth();
    int height = getHeight();
    const float centreX = 0.5f * width;
    const float centreY = 0.5f * height;
    const float radius = jmin(centreX, centreY) - 2.0f;
    const float sliderPos = (float)valueToProportionOfLength(getValue());

    g_.fillAll(Colour(background_colour_));

    static const float rotaryStartAngle = (float_Pi * 1.2f);
    static const float rotaryEndAngle = (float_Pi * 2.8f);

    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g_.setColour(Colour(_owner->_slider_fill_color));
    static const float thickness = 0.65; //* (1.f/40*slider.getWidth()); /* 0.7f; */

    {
        Path filledArc;
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
        g_.fillPath(filledArc);

        g_.setColour(Colour(_owner->_slider_knob_color));
        const float innerRadius = radius * thickness;
        Path realKnob;
        realKnob.addEllipse(-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
        g_.fillPath(realKnob, AffineTransform::rotation(angle).translated(centreX, centreY));
    }

    float innerRadius = radius * 0.2f;
    {
        g_.setColour(Colour(_owner->_slider_fill_color));
        Path p;
        p.addTriangle(-innerRadius, 0.0f, 0.0f, -radius * thickness * 1.f, innerRadius, 0.0f);

        p.addEllipse(-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
        g_.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
    }

    g_.setColour(Colour(_owner->_slider_outline_color));
    Path outlineArc;
    outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
    outlineArc.closeSubPath();
    g_.strokePath(outlineArc, PathStrokeType(1));
};

void UISlider::on_mouse_down(const MouseEvent &e_)
{
    if (_controller)
    {
        is_mouse_down = true;
        _controller->on_mouse_down(e_);
    }
}

void UISlider::on_long_mouse_down()
{
    if (_controller)
        _controller->on_long_mouse_down();
}

void UISlider::on_mouse_up(const MouseEvent &e_)
{
    if (_controller)
        if (is_mouse_down)
        {
            is_mouse_down = false;
            _controller->on_clicked();
            _controller->on_mouse_up(e_);
        }
}

void UISlider::sliderValueChanged(Slider *s_)
{
    if (_controller)
        _controller->on_value_changed(s_->getValue());
}

void UISlider::set_value(int value_, NotificationType n_type_)
{
    slider->setValue(value_, n_type_);
}

int UISlider::get_value() const { return slider->getValue(); }

void UISlider::setRange(int min_, int max_, int interval_)
{
    slider->setRange(min_, max_, interval_);
}

void UISlider::setTextBoxStyle(Slider::TextEntryBoxPosition box_pos_, bool is_read_only,
                               int text_box_width, int text_box_height)
{
    slider->setTextBoxStyle(box_pos_, is_read_only, text_box_width, text_box_height);
}

void UISlider::setSliderStyle(Slider::SliderStyle style_) { slider->setSliderStyle(style_); }

void UISlider::setDragSensitivity(int sensitivity_)
{
    slider->setMouseDragSensitivity(sensitivity_);
}

void UISlider::resized(int w_, int h_)
{
    this->setBounds(0, 0, w_, h_);
    slider->setBounds(0, 0, w_, h_);
}

void UISlider::cache_paint(Graphics &g_, uint32 background_colour_)
{
    slider->cache_paint(g_, background_colour_);
}

bool UISlider::use_long_mouse_down()
{
    if (_controller)
        return _controller->use_default_multi_drag();

    return false;
}

void UISlider::on_mouse_drag(const MouseEvent &)
{
    if (_controller)
    {
        if (_controller->should_start_multi_drag())
        {
            Image img(Image::RGB, 0, 0, false);
            startDragging(_controller->get_multi_dragNdrop_ident(), this,
                          _model->get_drag_image(img), true);
        }
    }
}

bool UISlider::isInterestedInDragSource(const DragAndDropTarget::SourceDetails &sd_)
{
    if (_controller)
        return _controller->is_interested_in_drag_source(sd_);

    return false;
}

void UISlider::itemDropped(const DragAndDropTarget::SourceDetails &sd_)
{
    if (_controller)
        _controller->item_dropped(sd_);
}

Component *UISlider::get_top_level_component() { return this; }

void *UISlider::get_top_level_impl() { return this; }
