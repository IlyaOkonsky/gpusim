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
            static const quint32 c_defaultGPUCoreRating;

            static const QString c_defaultResourceArch;
            static const QString c_defaultResourceOS;
            static const double  c_defaultResourceBaudRate;
            static const double  c_defaultResourceCostPerSec;
            static const double  c_defaultLinkBaudRate;

            static const double  c_defaultLimitationsDivider;
            static const double  c_defaultSmallTPBPenaltyWeight;
            static const double  c_defaultLargeTPBPenaltyWeight;
            static const double  c_defaultMultiplicativeLengthScaleFactor;
            static const double  c_defaultAdditiveLengthScaleFactor;

        public:
            CNBodyExperimentGeneratorSettings();

            bool operator == (const CNBodyExperimentGeneratorSettings& other) const;
            bool operator != (const CNBodyExperimentGeneratorSettings& other) const;

            bool operator < (const CNBodyExperimentGeneratorSettings& other) const;

            quint32 getGPUCoreRating() const;
            void setGPUCoreRating(quint32 gpuCoreRating);

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



            double getLimitationsDivider() const;
            void setLimitationsDivider(double limitationsDivider);

            double getSmallTPBPenaltyWeight() const;
            void setSmallTPBPenaltyWeight(double smallTPBPenaltyWeight);

            double getLargeTPBPenaltyWeight() const;
            void setLargeTPBPenaltyWeight(double largeTPBPenaltyWeight);

            double getMultiplicativeLengthScaleFactor() const;
            void setMultiplicativeLengthScaleFactor(double multiplicativeLengthScaleFactor);

            double getAdditiveLengthScaleFactor() const;
            void setAdditiveLengthScaleFactor(double additiveLengthScaleFactor);


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
            quint32 m_gpuCoreRating;

            QString m_resourceArch;
            QString m_resourceOS;
            double m_resourceBaudRate;
            double m_resourceCostPerSec;
            double m_linkBaudRate;

            double m_limitationsDivider;
            double m_smallTPBPenaltyWeight;
            double m_largeTPBPenaltyWeight;
            double m_multiplicativeLengthScaleFactor;
            double m_additiveLengthScaleFactor;

        private:
            static const QString c_className;
            static const QString c_gpuCoreRatingPropName;
            static const QString c_resourceArchPropName;
            static const QString c_resourceOSPropName;
            static const QString c_resourceBaudRatePropName;
            static const QString c_resourceCostPerSecPropName;
            static const QString c_linkBaudRatePropName;
            static const QString c_limitationsDividerPropName;
            static const QString c_smallTPBPenaltyWeightPropName;
            static const QString c_largeTPBPenaltyWeightPropName;
            static const QString c_multiplicativeLengthScaleFactorPropName;
            static const QString c_additiveLengthScaleFactorPropName;
        };
    }
}

