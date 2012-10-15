#pragma once

#include "GridSimMachineConfig.h"

namespace Core
{
    namespace GridSimConfig
    {
        class CGridSimResourceConfig: public Core::Serialization::IJavaXMLSerialize
        {
        public:
            static const double  c_minCostPerSec;
            static const double  c_minTimeZone;
            static const double  c_maxTimeZone;
            static const quint32 c_minAllocPolicy;
            static const quint32 c_maxAllocPolicy;
            static const double  c_minBaudRate;

            static const QString c_defaultName;
            static const QString c_defaultArch;
            static const QString c_defaultOS;
            static const double  c_defaultCostPerSec;
            static const double  c_defaultTimeZone;
            static const quint32 c_defaultAllocPolicy;
            static const double  c_defaultBaudRate;

        public:
            CGridSimResourceConfig(
                const QString &name = c_defaultName,
                const QString &arch = c_defaultArch,
                const QString &os   = c_defaultOS,
                double costPerSec   = c_defaultCostPerSec,
                double timeZone     = c_defaultTimeZone,
                qint32 allocPolicy  = c_defaultAllocPolicy,
                double baudRate     = c_defaultBaudRate,
                const CGridSimMachinesConfig &machines = CGridSimMachinesConfig());

            bool operator == (const CGridSimResourceConfig& other) const;
            bool operator != (const CGridSimResourceConfig& other) const;

            QString getName() const;
            void setName(const QString &name);

            QString getArch() const;
            void setArch(const QString &arch);

            QString getOS() const;
            void setOS(const QString &os);

            double getCostPerSec() const;
            void setCostPerSec(double costPerSec);

            double getTimeZone() const;
            void setTimeZone(double timeZone);

            qint32 getAllocPolicy() const;
            void setAllocPolicy(qint32 allocPolicy);

            double getBaudRate() const;
            void setBaudRate(double baudRate);

            CGridSimMachinesConfig getMachines() const;
            const CGridSimMachinesConfig &getMachinesRef() const;
            void setMachines(const CGridSimMachinesConfig &machines);

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
            static bool checkName(const QString &name);

        private:
            QString m_name;
            QString m_arch;
            QString m_os;
            double m_costPerSec;
            double m_timeZone;
            qint32 m_allocPolicy;
            double m_baudRate;
            CGridSimMachinesConfig m_machines;

        private:
            static const QString c_nameRegExp;

            static const QString c_className;
            static const QString c_namePropName;
            static const QString c_archPropName;
            static const QString c_osPropName;
            static const QString c_costPerSecPropName;
            static const QString c_timeZonePropName;
            static const QString c_allocPolicyPropName;
            static const QString c_baudRatePropName;
            static const QString c_machinesPropName;
        };

        typedef QVector<CGridSimResourceConfig> CGridSimResourcesConfig;
    }
}

Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimResourceConfig);