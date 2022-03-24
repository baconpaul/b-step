/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_D5B3C42D114CD2F2__
#define __JUCE_HEADER_D5B3C42D114CD2F2__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"
#include "UIEditorToolbar.h"
#include <juce_gui_extra/juce_gui_extra.h>
class AppInstanceStore;

//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorAppStyler : public UiEditor,
                          public juce::Timer,
                          public juce::Button::Listener,
                          public juce::Label::Listener
{
  public:
    //==============================================================================
    UiEditorAppStyler(AppInstanceStore *const app_instance_store_);
    ~UiEditorAppStyler();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    AppInstanceStore *const _app_instance_store;
    std::uint32_t *target_color;
    std::uint32_t last_col_refresh;

    void timerCallback() override;
    void on_close_clicked() override;

    void on_load_clicked() override;
    void on_save_clicked() override;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;
    void labelTextChanged(juce::Label *labelThatHasChanged) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<UiEditorToolbar> toolbar;
    std::unique_ptr<juce::ColourSelector> colour_selector;
    std::unique_ptr<juce::TextButton> col_1;
    std::unique_ptr<juce::TextButton> col_3;
    std::unique_ptr<juce::TextButton> col_4;
    std::unique_ptr<juce::TextButton> col_5;
    std::unique_ptr<juce::TextButton> col_6;
    std::unique_ptr<juce::TextButton> col_7;
    std::unique_ptr<juce::TextButton> col_8;
    std::unique_ptr<juce::TextButton> col_9;
    std::unique_ptr<juce::TextButton> col_10;
    std::unique_ptr<juce::TextButton> col_11;
    std::unique_ptr<juce::TextButton> col_12;
    std::unique_ptr<juce::TextButton> col_13;
    std::unique_ptr<juce::TextButton> col_14;
    std::unique_ptr<juce::TextButton> col_15;
    std::unique_ptr<juce::TextButton> col_16;
    std::unique_ptr<juce::Label> label_current_color;
    std::unique_ptr<juce::TextButton> copy;
    std::unique_ptr<juce::TextButton> past;
    std::unique_ptr<juce::TextButton> col_17;
    std::unique_ptr<juce::TextButton> col_18;
    std::unique_ptr<juce::TextButton> col_19;
    std::unique_ptr<juce::TextButton> rand;
    std::unique_ptr<juce::Label> list;
    std::unique_ptr<juce::TextButton> col_2;
    std::unique_ptr<juce::TextButton> col_20;
    std::unique_ptr<juce::TextButton> col_21;
    std::unique_ptr<juce::TextButton> col_22;
    std::unique_ptr<juce::TextButton> col_23;
    std::unique_ptr<juce::TextButton> style_1;
    std::unique_ptr<juce::TextButton> style_3;
    std::unique_ptr<juce::TextButton> style_4;
    std::unique_ptr<juce::TextButton> style_5;
    std::unique_ptr<juce::TextButton> button_info;
    std::unique_ptr<juce::TextButton> col_master;
    std::unique_ptr<juce::TextButton> reset;
    std::unique_ptr<juce::TextButton> rand2;
    std::unique_ptr<juce::TextButton> style_6;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UiEditorAppStyler)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_D5B3C42D114CD2F2__
