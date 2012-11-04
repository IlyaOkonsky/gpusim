#pragma once

#include "../Serialization/IJavaXMLSerialize.h"

#include <QVector>
#include <QMetaType>

namespace Core
{
    namespace GridSimConfig
    {
        class GPUSIM_FE_CORE_EXPORT CGridSimGridletConfig: public Core::Serialization::IJavaXMLSerialize
        {
        public:
            static const double  c_minLength;
            static const quint64 c_minInputSize;
            static const quint64 c_minOutputSize;

            static const quint32 c_defaultID;
            static const double  c_defaultLength;
            static const quint64 c_defaultInputSize;
            static const quint64 c_defaultOutputSize;

        public:
            CGridSimGridletConfig(quint32 id = c_defaultID, double length = c_defaultLength,
                quint64 inputSize = c_defaultInputSize, quint64 outputSize = c_defaultOutputSize);

            bool operator == (const CGridSimGridletConfig& other) const;
            bool operator != (const CGridSimGridletConfig& other) const;

            quint32 getID() const;
            void setID(quint32 id);

            double getLength() const;
            void setLength(double length);

            quint64 getInputSize() const;
            void setInputSize(quint64 inputSize);

            quint64 getOutputSize() const;
            void setOutputSize(quint64 outputSize);

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
            double m_length;
            quint64 m_inputSize;
            quint64 m_outputSize;

        private:
            static const QString c_className;
            static const QString c_idPropName;
            static const QString c_lengthPropName;
            static const QString c_inputSizePropName;
            static const QString c_outputSizePropName;
        };

        typedef QVector<CGridSimGridletConfig> CGridSimGridletsConfig;
    }
}

Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimGridletConfig);
Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimGridletsConfig);