/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "glk/glk.h"
#include "glk/dialogs.h"

#include "gui/ThemeEval.h"
#include "gui/gui-manager.h"
#include "gui/message.h"
#include "gui/saveload.h"
#include "gui/ThemeEngine.h"
#include "gui/widget.h"
#include "gui/widgets/list.h"
#include "gui/widgets/popup.h"

#include "glk/conf.h"
#include "glk/fonts.h"
#include "glk/screen.h"
#include "glk/windows.h"
#include "glk/window_text_buffer.h"
#include "glk/window_text_grid.h"

#include "common/system.h"
#include "common/translation.h"

namespace Glk {

GlkOptionsWidget::GlkOptionsWidget(GuiObject *boss, const Common::String &name, const Common::String &domain)
    : GUI::OptionsContainerWidget(boss, name, "GlkOptionsDialog", domain) {

    _tfontPopUps[0] = new GUI::PopUpWidget(widgetsBoss(), "GlkOptionsDialog.TFont0");
    
    GUI::StaticTextWidget *tfontlbl = new GUI::StaticTextWidget(widgetsBoss(), "GlkOptionsDialog.TFont", _("Text font:"), _("Choose which font you'd like to see in story/dialogue text. We've got a lot of entries and if you want, you can also select one of your configured custom fonts if you've added any via the engine's configuration file."));
    tfontlbl->setAlign(Graphics::kTextAlignRight);
    
    for (int f = MONOR; f <= CUSTOM2; ++f) {
        _tfontPopUps[0]->appendEntry(Screen::getFontName((FACES)f), f);
    }
    
    _gfontPopUps[0] = new GUI::PopUpWidget(widgetsBoss(), "GlkOptionsDialog.GFont0");

    GUI::StaticTextWidget *gfontlbl = new GUI::StaticTextWidget(widgetsBoss(), "GlkOptionsDialog.GFont", _("Grid font"), _("Choose which font you'd like to see for drawn graphics such as maps, diagrams, puzzles, etc."));
    gfontlbl->setAlign(Graphics::kTextAlignRight);

    for (int f = MONOR; f <= CUSTOM2; ++f) {
        _gfontPopUps[0]->appendEntry(Screen::getFontName((FACES)f), f);
    }

    _tcolorPopUps[0] = new GUI::PopUpWidget(widgetsBoss(), "GlkOptionsDialog.TColor0");

    GUI::StaticTextWidget *tcolorlbl = new GUI::StaticTextWidget(widgetsBoss(), "GlkOptionsDialog.TColor", _("Text color"), _("Choose the color you'd like to see in story/dialogue text. If you want, you can also choose your custom color by entering it's hex value in your configuration file."));
    tcolorlbl->setAlign(Graphics::kTextAlignRight);

    static const char *fontColorNames[] = { "normal", "alternate", "override" };

    for (int c = GUI::ThemeEngine::kFontColorNormal; c <= GUI::ThemeEngine::kFontColorOverride; ++c) {
        _tcolorPopUps[0]->appendEntry(fontColorNames[c], c);
    }

    _gcolorPopUps[0] = new GUI::PopUpWidget(widgetsBoss(), "GlkOptionsDialog.GColor0");

    GUI::StaticTextWidget *gcolorlbl = new GUI::StaticTextWidget(widgetsBoss(), "GlkOptionsDialog.GColor", _("Grid color"), _("Choose the color you'd like the engine show you for drawn graphics such as maps, diagrams, puzzles, etc. If you want, you can also choose your custom color by entering it's hex value in your configuration file."));
    gcolorlbl->setAlign(Graphics::kTextAlignRight);

    static const char *gridColorNames[] = { "normal", "alternate", "override" };

    for (int c = GUI::ThemeEngine::kFontColorNormal; c <= GUI::ThemeEngine::kFontColorOverride; ++c) {
        _gcolorPopUps[0]->appendEntry(gridColorNames[c], c);
    }
}

GlkOptionsWidget::~GlkOptionsWidget() = default;


void GlkOptionsWidget::load() {
    if (g_conf) {
        if (_tfontPopUps[0]) {
        FACES tf = g_conf->_tStyles[style_Normal].font;
        _tfontPopUps[0]->setSelectedTag(tf);
        }
    }

    else {
        if (_tfontPopUps[0]) {
            _tfontPopUps[0]->setSelectedTag(Screen::getFontId(ConfMan.get("tfont", _domain)));
        }
    }

    if (g_conf) {
        if (_gfontPopUps[0]) {
        FACES gf = g_conf->_gStyles[style_Normal].font;
        _gfontPopUps[0]->setSelectedTag(gf);
        }
    }

    else {
        if (_gfontPopUps[0]) {
            _gfontPopUps[0]->setSelectedTag(Screen::getFontId(ConfMan.get("tfont", _domain)));
        }
    }

    if (_tcolorPopUps[0]) {
        Common::U32String s = ConfMan.get("tcolor", _domain);
        GUI::ThemeEngine::FontColor col = GUI::ListWidget::getThemeColor(s);

        _tcolorPopUps[0]->setSelectedTag((uint32)col);
    }

    if (_gcolorPopUps[0]) {
        Common::U32String s = ConfMan.get("gcolor", _domain);
        GUI::ThemeEngine::FontColor col = GUI::ListWidget::getThemeColor(s);

        _gcolorPopUps[0]->setSelectedTag((uint32)col);
    }
}

void GlkOptionsWidget::defineLayout(GUI::ThemeEval &layouts,
                                    const Common::String &layoutName,
                                    const Common::String &overlayedLayout) const {
    layouts.addDialog(layoutName, overlayedLayout)
        		.addLayout(GUI::ThemeLayout::kLayoutVertical)
            		.addLayout(GUI::ThemeLayout::kLayoutHorizontal)
                        .addPadding(0, 0, 0, 0)
                		.addWidget("TFont", "OptionsLabel")
                		.addWidget("TFont0", "PopUp")
                    .closeLayout()
                    .addLayout(GUI::ThemeLayout::kLayoutHorizontal)
                        .addPadding(0, 0, 0, 0)
                        .addWidget("GFont", "OptionsLabel")
                        .addWidget("GFont0", "PopUp")
                    .closeLayout()
                    .addLayout(GUI::ThemeLayout::kLayoutHorizontal)
                        .addPadding(0, 0, 0, 0)
                        .addWidget("TColor", "OptionsLabel")
                        .addWidget("TColor0", "PopUp")
                    .closeLayout()
                    .addLayout(GUI::ThemeLayout::kLayoutHorizontal)
                        .addPadding(0, 0, 0, 0)
                        .addWidget("GColor", "OptionsLabel")
                        .addWidget("GColor0", "PopUp")
                    .closeLayout()
                .closeLayout()
        	.closeDialog();
}

bool GlkOptionsWidget::save() {
    if (!g_conf) {
        return true;
    }

    if (_tfontPopUps[0]) {
        FACES tf = (FACES)_tfontPopUps[0]->getSelectedTag();
        for (int i = 0; i < style_NUMSTYLES; ++i) {
            g_conf->_tStyles[i].font = tf;
            Common::String key = Common::String::format("tfont_%d", i);
            ConfMan.set(key, Screen::getFontName(tf), _domain);
        }
    }

    if(_gfontPopUps[0]) {
        FACES gf = (FACES)_gfontPopUps[0]->getSelectedTag();
        for (int i = 0; i < style_NUMSTYLES; ++i) {
            g_conf->_gStyles[i].font = gf;
            Common::String key = Common::String::format("gfont_%d", i);
            ConfMan.set(key, Screen::getFontName(gf), _domain);
        }
    }

    if (g_vm && g_vm->_windows) {
        for (Glk::Windows::iterator it = g_vm->_windows->begin(); it != g_vm->_windows->end(); ++it) {
            Glk::Window *w = *it;
            if (auto tb = dynamic_cast<Glk::TextBufferWindow *>(w)) {
                tb->refreshStyles();
            }
            else if (auto tg = dynamic_cast<Glk::TextGridWindow *>(w)) {
                tg->refreshStyles();
            }
        }
        g_vm->_windows->_forceRedraw = true;
    }
    
    if(_tcolorPopUps[0]) {
        auto col = (GUI::ThemeEngine::FontColor)_tcolorPopUps[0]->getSelectedTag();
        const char *name;
        switch (col) {
        case GUI::ThemeEngine::kFontColorNormal:
            name = "normal";
            break;
        case GUI::ThemeEngine::kFontColorAlternate:
            name = "alternate";
            break;
        case GUI::ThemeEngine::kFontColorOverride:
            name = "override";
            break;
        default:
            name = "normal";
        }
        ConfMan.set("tcolor", name, _domain);
    }

    if(_gcolorPopUps[0]) {
        auto col = (GUI::ThemeEngine::FontColor)_gcolorPopUps[0]->getSelectedTag();
        const char *name;
        switch (col) {
        case GUI::ThemeEngine::kFontColorNormal:
            name = "normal";
            break;
        case GUI::ThemeEngine::kFontColorAlternate:
            name = "alternate";
            break;
        case GUI::ThemeEngine::kFontColorOverride:
            name = "override";
            break;
        default:
            name = "normal";
        }
        ConfMan.set("gcolor", name, _domain);
    }

    return true;
}

} // End of namespace Glk
