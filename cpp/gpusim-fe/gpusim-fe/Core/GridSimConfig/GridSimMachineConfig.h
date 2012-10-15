#pragma once

#include "../Serialization/IJavaXMLSerialize.h"

#include <QVector>
#include <QMetaType>

namespace Core
{
    namespace GridSimConfig
    {
        class CGridSimMachineConfig: public Core::Serialization::IJavaXMLSerialize
        {
        public:
            static const quint32 c_minPECount;
            static const quint32 c_minPERating;

            static const quint32 c_defaultlID;
            static const quint32 c_defaultPECount;
            static const quint32 c_defaultPERating;
        public:
            CGridSimMachineConfig(quint32 id = c_defaultlID, quint32 peCount = c_defaultPECount,
                quint32 peRating = c_defaultPERating);

            bool operator == (const CGridSimMachineConfig& other) const;
            bool operator != (const CGridSimMachineConfig& other) const;

            quint32 getID() const;
            void setID(quint32 id);

            quint32 getPECount() const;
            void setPECount(quint32 peCount);

            quint32 getPERating() const;
            void setPERating(quint32 peRating);

            virtual bool isValid() const;

        protected:
            // You must overload this stubs for data (de)serialization.
            //
            virtual void saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const;
            virtual void loadDataFromXMLNode(const QDomElement &elem);

            // Factory method for creating instances of derived objects
            virtual Serialization::IJavaXMLSerializePtr create() const;

            // Method from copying data from other instance of derived object
            virtual void copy(const Serialization::IJavaXMLSerializePtr &from);

        private:
            quint32 m_id;
            quint32 m_peCount;
            quint32 m_peRating;

        private:
            static const QString c_className;
            static const QString c_idPropName;
            static const QString c_peCountPropName;
            static const QString c_peRatingPropName;
        };

        typedef QVector<CGridSimMachineConfig> CGridSimMachinesConfig;
    }
}

Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimMachineConfig);
Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimMachinesConfig);