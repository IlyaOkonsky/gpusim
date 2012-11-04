#pragma once

#include "GridSimGridletOutput.h"

namespace Core
{
    namespace GridSimConfig
    {
        class GPUSIM_FE_CORE_EXPORT CGridSimOutput: public Core::Serialization::IJavaXMLSerialize
        {
        public:
            static const quint32 c_minVersion;
            static const double  c_minTotalSimulationTime;

            static const quint32 c_currentOutputVersion;
            static const QString c_defaultName;
            static const double  c_defaultTotalSimulationTime;

        public:
            CGridSimOutput(const QString &name = c_defaultName,
                double totalSimulationTime = c_defaultTotalSimulationTime,
                const CGridSimGridletsOutput &gridlets = CGridSimGridletsOutput());

            bool operator == (const CGridSimOutput& other) const;
            bool operator != (const CGridSimOutput& other) const;

            quint32 getVersion() const;
            void setVersion(quint32 version);

            QString getName() const;
            void setName(const QString &name);

            double getTotalSimulationTime() const;
            void setTotalSimulationTime(double totalSimulationTime);

            CGridSimGridletsOutput getGridlets() const;
            const CGridSimGridletsOutput &getGridletsRef() const;
            void setGridlets(const CGridSimGridletsOutput &gridlets);

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
            quint32 m_version;
            QString m_name;
            double m_totalSimulationTime;
            CGridSimGridletsOutput m_gridlets;

        private:
            static const QString c_className;
            static const QString c_versionPropName;
            static const QString c_namePropName;
            static const QString c_totalSimulationTimePropName;
            static const QString c_gridletsPropName;
        };
    }
}

Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimOutput);