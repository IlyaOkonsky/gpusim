#pragma once

#include "../Serialization/IJavaXMLSerialize.h"

namespace Core
{
    namespace Settings
    {
        class GPUSIM_FE_CORE_EXPORT CNBodyExperimentGeneratorSettings:
            public Core::Serialization::IJavaXMLSerialize
        {
        public:
            static const quint32 c_defaultCPUMachinePECount;
            static const quint32 c_defaultCPUMachinePERating;
            static const quint32 c_defaultGPUMachinePECount;
            static const quint32 c_defaultGPUMachinePERating;

            static const QString c_defaultResourceArch;
            static const QString c_defaultResourceOS;
            static const double  c_defaultResourceBaudRate;
            static const double  c_defaultResourceCostPerSec;
            static const double  c_defaultLinkBaudRate;

            static const double  c_defaultLoadOperationCost;
            static const double  c_defaultSaveOperationCost;

        public:
            CNBodyExperimentGeneratorSettings();

            bool operator == (const CNBodyExperimentGeneratorSettings& other) const;
            bool operator != (const CNBodyExperimentGeneratorSettings& other) const;

            bool operator < (const CNBodyExperimentGeneratorSettings& other) const;

            quint32 getCPUMachinePECount() const;
            void setCPUMachinePECount(quint32 cpuMachineCount);

            quint32 getCPUMachinePERating() const;
            void setCPUMachinePERating(quint32 cpuMachineRating);

            quint32 getGPUMachinePECount() const;
            void setGPUMachinePECount(quint32 gpuMachineCount);

            quint32 getGPUMachinePERating() const;
            void setGPUMachinePERating(quint32 gpuMachineRating);

            QString getResourceArch() const;
            void setResourceArch(const QString &resourceArch);

            QString getResourceOS() const;
            void setResourceOS(const QString &resourceOS);

            double getResourceBaudRate() const;
            void setResourceBaudRate(double resourceBaudRate);

            double getResourceCostPerSec() const;
            void setResourceCostPerSec(double resourceCostPerSec);

            double getLinkBaudRate() const;
            void setLinkBaudRate(double linkBaudRate);

            double getLoadOperationCost() const;
            void setLoadOperationCost(double loadOperationCost);

            double getSaveOperationCost() const;
            void setSaveOperationCost(double saveOperationCost);

            bool saveToXMLFile(const QString &filePath) const;
            bool loadFromXMLFile(const QString &filePath);

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
            quint32 m_cpuMachinePECount;
            quint32 m_cpuMachinePERating;
            quint32 m_gpuMachinePECount;
            quint32 m_gpuMachinePERating;

            QString m_resourceArch;
            QString m_resourceOS;
            double m_resourceBaudRate;
            double m_resourceCostPerSec;
            double m_linkBaudRate;

            double m_loadOperationCost;
            double m_saveOperationCost;

        private:
            static const QString c_className;
            static const QString c_cpuMachinePECountPropName;
            static const QString c_cpuMachinePERatingPropName;
            static const QString c_gpuMachinePECountPropName;
            static const QString c_gpuMachinePERatingPropName;
            static const QString c_resourceArchPropName;
            static const QString c_resourceOSPropName;
            static const QString c_resourceBaudRatePropName;
            static const QString c_resourceCostPerSecPropName;
            static const QString c_linkBaudRatePropName;
            static const QString c_loadOperationCostPropName;
            static const QString c_saveOperationCostPropName;
        };
    }
}

