/*
  ==============================================================================

    Created: late
    Author:  monotomy

  ==============================================================================
*/

#ifndef CORE_STEP_CONTROLLERS_INCLUDED
#define CORE_STEP_CONTROLLERS_INCLUDED

#include "CoreObservers.h"
#include "ControllerConfig.h"

// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
class ControllerStepMute : public MONO_UIButtonController
{
    SelectedStepObserver selected_step;

    USE_DEFAULT_MULTI_DRAG

    PodParameterBase *get_parameter() const override { return &selected_step.get().is_mute; }

    unsigned int get_current_state() const override
    {
        if (selected_step.get_selected_bar().mute || selected_step.get_selected_bar().skip)
        {
            if (selected_step.get().is_mute)
                return STATES::ON_2;
            else
                return STATES::OFF_2;
        }
        else
        {
            if (selected_step.get_selected_bar().barstep(selected_step.step_id).mute ||
                selected_step.get_selected_bar().barstep(selected_step.step_id).skip)
            {
                if (selected_step.get().is_mute)
                    return STATES::ON_2;
                else
                    return STATES::OFF_2;
            }
            else if (selected_step.get().is_mute)
                return STATES::ON_1;
            else
                return STATES::OFF_1;
        }
    }

    void get_label_text_top(String &string_) const override
    {
        int8 sum_octave_offset = selected_step.get_selected_barstep().octave_offset;
        sum_octave_offset += selected_step.get_selected_barstring().octave_offset;
        sum_octave_offset +=
            _app_instance_store->pattern.master_string_octave(selected_step.barstring_id).value();

        if (sum_octave_offset != 0)
        {
            string_ = String(sum_octave_offset);
            return;
        }

        // TODO can be set by the caller
        string_ = HAS_NO_TEXT_VALUE;
    }

    AppStyle *get_custom_label_style() const override
    {
        return _app_instance_store->style_step_area_octave;
    }

    uint32 get_current_color() const override
    {
        if (_model)
        {
            unsigned int state_value = get_current_state();

            // USER DEFINED IMPL
            switch (state_value)
            {
            case STATES::OFF_1:
                return _model->get_style()->get_state_off_1_color();
            case STATES::ON_1:
                return get_runing_color();
            case STATES::OFF_2:
                return _model->get_style()->get_state_off_2_color();
            case STATES::ON_2:
                return _model->get_style()->get_state_on_2_color();
            default:
                return 0x00000000;
            }
        }
        return 0x00000000;
    }

    uint32 get_runing_color() const
    {
        if (selected_step.get_selected_barstep().use_step_chord)
        {
            return _app_instance_store->style_step_area_chord->get_state_on_1_color();
        }
        else
        {
            return _model->get_style()->get_state_on_1_color();
        }
    }

  public:
    ControllerStepMute(AppInstanceStore *const app_instance_store_, uint8 barstring_id_,
                       uint8 step_id_)
        : MONO_UIButtonController(app_instance_store_),
          selected_step(app_instance_store_, barstring_id_, step_id_)
    {
    }
};

#endif
