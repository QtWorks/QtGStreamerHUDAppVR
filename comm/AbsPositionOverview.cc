#include "AbsPositionOverview.h"

AbsPositionOverview::AbsPositionOverview(QObject *parent) :
    QObject(parent)
{
    m_timeUsec = 0;
    m_lat = 0;
    m_lon = 0;
    m_alt = 0;
    m_latHome = 0;
    m_lonHome = 0;
    m_altHome = 0;
    m_eph = 0;
    m_epv = 0;
    m_vel = 0;
    m_cog = 0;
    m_fixType = 0;
    m_satellitesVisible = 0;

    m_timeBootMs = 0;
    m_relativeAlt = 0;
    m_vx = 0;
    m_vy = 0;
    m_vz = 0;
    m_hdg = 0;
    m_homeHeading = 0;
    m_distToHome = 0;
}

void AbsPositionOverview::parseGpsRawInt(LinkInterface *link, const mavlink_message_t &message, const mavlink_gps_raw_int_t &state)
{
    Q_UNUSED(link);
    Q_UNUSED(message);

    this->setFixType(state.fix_type);
    this->setSatellitesVisible(state.satellites_visible);

    if (state.fix_type > 2)
    {
        // First time set home
        if (state.satellites_visible > 6 && m_latHome == 0)
        {
            this->setLatHome(state.lat);
            this->setLonHome(state.lon);
            this->setAltHome(state.alt);
        }

        if (m_latHome != 0)
        {
            this->setLat(state.lat);
            this->setLon(state.lon);
            this->setAlt(state.alt);
            this->setVel(state.vel);

            // Compute home bearing
            // shrinking factor for longitude going to poles direction
            double rads = (double)abs(this->getLatHome()) * 0.0174532925;
            double scaleLongUp = 1.0f / cos(rads);
            double scaleLongDown = cos(rads);

            //DIR to Home
            double dstlon = (this->getLonHome() - state.lon); //OffSet_X
            double dstlat = (this->getLatHome() - state.lat) * scaleLongUp; //OffSet Y
            double bearing = (90 +  (atan2(dstlat, -dstlon) * 57.295775)) + 180; //absolut home direction
            if (bearing < 0) bearing += 360;//normalization
            if (bearing > 360) bearing -= 360;
            this->setHomeHeading(bearing);

            // DIST to Home
            dstlat = abs(this->getLatHome() - state.lat)*.01113195;
            dstlon = abs(this->getLonHome() - state.lon)*.01113195*scaleLongDown;
            double dist = sqrt((dstlat*dstlat) + (dstlon*dstlon));
            this->setDistToHome(dist);
        }
    }
}

void AbsPositionOverview::parseGlobalPositionInt(LinkInterface *link, const mavlink_message_t &message, const mavlink_global_position_int_t &state)
{
    Q_UNUSED(link);
    Q_UNUSED(message);
    this->setRelativeAlt(state.relative_alt/1000.0);
}

void AbsPositionOverview::messageReceived(LinkInterface* link,mavlink_message_t message)
{
    switch (message.msgid)
    {
        case MAVLINK_MSG_ID_GPS_RAW_INT:
        {
            mavlink_gps_raw_int_t state;
            mavlink_msg_gps_raw_int_decode(&message, &state);
            parseGpsRawInt(link,message,state);
            break;
        }
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        {
            mavlink_global_position_int_t state;
            mavlink_msg_global_position_int_decode(&message, &state);
            parseGlobalPositionInt(link,message,state);
            break;
        }
    }
}
