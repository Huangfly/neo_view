/* ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 * ┃     ┏━┣━┣┓　　┏┓┏┓┳┓┏━━┓┣┣━┓　┓　┓┣┳━┓       ┃
 * ┃     ┏┏┏╯━┓┳┳━┛┏╯┃┃┃　　┃┣┣━┓┃┃　┃┃┃　　       ┃
 * ┃     ┃┃┏━╮┃┗┗┏╯┗┃┃╯┃　　┃┏┣━┓┃┃　┃╯┣━┓       ┃
 * ┃     ╰┫┏━┻╯┗┳┣┛┏┛┃┃┣━━┫┃┃　┃┃┃┗╯　┃　　       ┃
 * ┃     ┏┫━┳━┫┏┃┣┓┗┃┃╯┃　　┃┃┃　┃　┃　┃　┣━┓       ┃
 * ┃     ┗┗┗━━╯┗┛┛╯┗╯╰　┗━━╯　┛　┛┗╯　╰┛┗　　       ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃                     Copyright (c) 2013 jiangcaiyang                    ┃
 * ┃ This software is provided 'as-is', without any express or implied      ┃
 * ┃ warranty. In no event will the authors be held liable for any damages  ┃
 * ┃ arising from the use of this software.                                 ┃
 * ┃                                                                        ┃
 * ┃ Permission is granted to anyone to use this software for any purpose,  ┃
 * ┃ including commercial applications, and to alter it and redistribute it ┃
 * ┃ freely, subject to the following restrictions:                         ┃
 * ┃                                                                        ┃
 * ┃ 1. The origin of this software must not be misrepresented; you must    ┃
 * ┃    not claim that you wrote the original software. If you use this     ┃
 * ┃    software in a product, an acknowledgment in the product             ┃
 * ┃    documentation would be appreciated but is not required.             ┃
 * ┃ 2. Altered source versions must be plainly marked as such, and must    ┃
 * ┃    not be misrepresented as being the original software.               ┃
 * ┃ 3. This notice may not be removed or altered from any source           ┃
 * ┃    distribution.                                                       ┃
 * ┃                                                                        ┃
 * ┃ jiangcaiyang jiangcaiyang123@163.com                                   ┃
 * ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
 * ┃ file name: Camera.cpp                                                  ┃
 * ┃ create date: 2013年9月21日星期六 20时5分30秒                           ┃
 * ┃ last modified date: 2013年9月24日星期二 22时12分23秒                   ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 */
#include <QtOpenGL>
#include "Camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define PI 3.141592653

/*---------------------------------------------------------------------------*/
Camera::Camera( void )
{
    m_Pos = QVector3D( 0, 0, 0 );
    m_RotateX = 0.0f;
    m_RotateY = 0.0;
    m_RotateH = 0.0;
    m_Len = QVector3D( 0, 0, 0 );
    m_TryPos = QVector3D( 0, 0, 0 );
    m_TryRotateY = 0.0;
    m_TryRotateH = 0.0;
    m_HorizontalAngle = 0.0f;
    m_Try = false;
}

void Camera::reset()
{
    m_Pos = QVector3D( 0, 0, 0 );
    m_RotateX = 0.0f;
    m_RotateY = 0.0;
    m_RotateH = 0.0;
    m_Len = QVector3D( 0, 0, 0 );
    m_TryPos = QVector3D( 0, 0, 0 );
    m_TryRotateY = 0.0;
    m_TryRotateH = 0.0;
    m_HorizontalAngle = 0.0f;
    m_Try = false;
}
/*---------------------------------------------------------------------------*/
void Camera::SetPos( const QVector3D pos, bool _try )
{
    m_Try = _try;

    if ( m_Try ) m_TryPos = pos;
    else
    {
        m_Pos += pos;
        m_Offset2D += QVector3D( pos.x( ), pos.y( ), 0 );
        m_TryPos = QVector3D( 0, 0, 0 );
    }
}

void Camera::SetRotateX(float rotateX)
{
     m_RotateX += (rotateX/m_Win_Width*PI);
     if( m_RotateX >= PI*2.0f ){
         m_RotateX = 0.0f;
     }
}

void Camera::SetHorizontalAngle(float rotate)
{
    m_HorizontalAngle += (rotate/m_Win_Width*PI);
    if( m_HorizontalAngle >= PI*2.0f ){
        m_HorizontalAngle = 0.0f;
    }else if(m_HorizontalAngle < 0.0f){
        m_HorizontalAngle = PI*2.0f;
    }
}

void Camera::SetRotateY( float rotateY, bool _try )
{
    m_Try = _try;

    if ( m_Try ) m_TryRotateY = rotateY;
    else
    {
        m_RotateY += rotateY;
        m_TryRotateY = 0.0;
    }
}

void Camera::SetRotateH( float rotateH, bool _try )
{
    m_Try = _try;

    if ( m_Try ) m_TryRotateH = rotateH;
    else
    {
        m_RotateH += rotateH;
        m_TryRotateH = 0.0;
    }
}
/*---------------------------------------------------------------------------*/
void Camera::Apply( void )
{
    /*QVector3D ha = QVector3D::crossProduct( m_Pos -
                                            QVector3D( 0, m_Pos.y( ), 0 ) -
                                            m_Offset2D,
                                            QVector3D( 0, 1, 0 ) );*/
    QVector3D eyes = m_Pos + m_TryPos + m_Len;
    QVector3D focus = QVector3D( float(eyes.x()),// + eyes.z()*cos(m_RotateX)),
                                 float(eyes.y()),// + eyes.z()*sin(m_RotateX)),
                                 0.0f);
    float atX = sin(m_HorizontalAngle);
    float atY = cos(m_HorizontalAngle);
    gluLookAt(eyes.x(),eyes.y(),eyes.z(),
              focus.x(),focus.y(),0.0f,
              atX,atY,0);
}
/*---------------------------------------------------------------------------*/
void Camera::ZoomIn( float step )
{

    QVector3D tangent = m_Pos - QVector3D( m_Pos.x(), m_Pos.y(), 0 );

    m_Len -= tangent.normalized( ) * step;
    if( (m_Pos.z() + m_Len.z()) <= 0 ){
        m_Len += tangent.normalized( ) * step;
    }


}
/*---------------------------------------------------------------------------*/
void Camera::SetPosFromWindows(int diff_x, int diff_y)
{
    float _diff_x = diff_x;
    float _diff_y = diff_y;
    WindowsCoordinateToWorldCoordinate( _diff_x, _diff_y );
    QVector3D diff = QVector3D( _diff_x, _diff_y, 0.0f );
    m_Pos += diff;
}

void Camera::ScreenToWindowsCoordinate( float &x, float &y){
    x = (x-this->m_Win_Width*0.5f);
    y = -(y-this->m_Win_Height*0.5f);
}

void Camera::WindowsCoordinateToWorldCoordinate(float &x, float &y)
{
    float scale = GetZoom();
    float _x = x*scale;
    float _y = y*scale;

    float _sinAngle = sin(-m_HorizontalAngle);
    float _cosAngle = cos(-m_HorizontalAngle);

     y = _y*_cosAngle + _x*_sinAngle;// y = R*sin(a+b) = R*sina*cosb + R*cosa*sinb;
     x = _x*_cosAngle - _y*_sinAngle;// x = R*cos(a+b) = R*cosa*cosb - R*sina*sinb;

}

QVector2D Camera::GetPosFromWindows(int pos_x, int pos_y)
{
    float cursor_coordinate_x = pos_x;
    float cursor_coordinate_y = pos_y;
    ScreenToWindowsCoordinate( cursor_coordinate_x, cursor_coordinate_y );
    WindowsCoordinateToWorldCoordinate( cursor_coordinate_x, cursor_coordinate_y );
    //printf("wh:%f,%f\n",m_Win_Width,m_Win_Height);
    //printf("scale:%f\n",GetZoom());
    //printf("pos:%f %f %f\n",m_Pos.x(),m_Pos.y(),m_Pos.z());
    //printf("Len:%f %f %f\n",m_Len.x(),m_Len.y(),m_Len.z());
    QVector2D cur_pos = m_Pos.toVector2D() + m_Len.toVector2D() + QVector2D( cursor_coordinate_x, cursor_coordinate_y );

    return cur_pos;
}

