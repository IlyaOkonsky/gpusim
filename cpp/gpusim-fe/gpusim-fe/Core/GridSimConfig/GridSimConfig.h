#pragma once

#include "GridSimResourceConfig.h"
#include "GridSimGridletConfig.h"

namespace Core
{
    namespace GridSimConfig
    {
        class CGridSimConfig: public Core::Serialization::IJavaXMLSerialize
        {
        public:
            static const quint32 c_minVersion;
            static const double  c_minLinkBaudRate;

            static const quint32 c_currentConfigVersion;
            static const QString c_defaultName;
            static const double  c_defaultLinkBaudRate;

        public:
            CGridSimConfig(const QString &name = c_defaultName,
                double linkBaudRate = c_defaultLinkBaudRate,
                const CGridSimResourcesConfig &resources =  CGridSimResourcesConfig(),
                const CGridSimGridletsConfig &gridlets = CGridSimGridletsConfig());

            bool operator == (const CGridSimConfig& other) const;
            bool operator != (const CGridSimConfig& other) const;

            quint32 getVersion() const;
            void setVersion(quint32 version);

            QString getName() const;
            void setName(const QString &name);

            double getLinkBaudRate() const;
            void setLinkBaudRate(double linkBaudRate);

            CGridSimResourcesConfig getResources() const;
            const CGridSimResourcesConfig &getResourcesRef() const;
            void setResources(const CGridSimResourcesConfig &resources);

            CGridSimGridletsConfig getGridlets() const;
            const CGridSimGridletsConfig &getGridletsRef() const;
            void setGridlets(const CGridSimGridletsConfig &gridlets);

            static CGridSimConfig createTestConfig();

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
            double m_linkBaudRate;
            CGridSimResourcesConfig m_resources;
            CGridSimGridletsConfig m_gridlets;

        private:
            static const QString c_className;
            static const QString c_versionPropName;
            static const QString c_namePropName;
            static const QString c_linkBaudRatePropName;
            static const QString c_resourcesPropName;
            static const QString c_gridletsPropName;
        };
    }
}

Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimConfig);