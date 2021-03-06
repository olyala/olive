/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QSettings>
#include <QWidget>

class StyleDescriptor {
public:
  StyleDescriptor(const QString& name, const QString& path);

  const QString& name() const;
  const QString& path() const;

private:
  QString name_;
  QString path_;
};

class StyleManager : public QObject {
public:
  StyleManager();

  static StyleDescriptor DefaultStyle();

  static void SetStyle(const StyleDescriptor& style);

  static void SetStyle(const QString& style_path);

  static QList<StyleDescriptor> ListInternal();

private:
  static QPalette ParsePalette(const QString& ini_path);

  static void ParsePaletteGroup(QSettings* ini, QPalette* palette, QPalette::ColorGroup group);

  static void ParsePaletteColor(QSettings* ini, QPalette* palette, QPalette::ColorGroup group, const QString& role_name);
};

#endif // STYLEMANAGER_H
