/* *************************************************************************
 *  Copyright 2012 Jakob Gruber                                            *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ************************************************************************* */


#ifndef RENDERER_H
#define RENDERER_H

#include <boost/shared_ptr.hpp>
#include <QPixmap>
#include <QtSvg/QSvgRenderer>
#include <QString>
#include <QVector>

class Renderer
{
public:
    enum Resource {
        Transparent = 0,
        Background,
        CellFrame,
        Box,
        Cross,
        Highlight,
        Streak1,
        Streak2,
        Divider
    };

    enum FontSize {
        Tiny,
        Small,
        Regular,
        Large,
        Huge
    };

    static Renderer *instance();

    QPixmap getPixmap(Renderer::Resource res) const;

    int getTilesize() const;
    int getFontSize(enum FontSize) const;

    /* returns the width of streak areas in pixels */
    int getYOffset() const;
    int getXOffset() const;

    /* returns the width of streak areas in tiles */
    int getStreakGridCount() const;

    /* 0 < tilesize */
    void setTilesize(int tilesize);

private:
    Renderer();

    static boost::shared_ptr<QPixmap> load(QString path);
    QPixmap getCachedPixmap(Renderer::Resource resource, int h, int w) const;

private:

    int m_tilesize;
    const int m_streak_grid_count;

    boost::shared_ptr<QSvgRenderer> m_renderer;
    boost::shared_ptr<QPixmap> m_background;

    QVector<QString> m_names;
};

#endif // RENDERER_H
