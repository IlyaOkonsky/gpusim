#pragma once

#include "../Serialization/IJavaXMLSerialize.h"

#include <QVector>
#include <QMetaType>

namespace Core
{
    namespace GridSimConfig
    {
        class CGridSimGridletOutput: public Core::Serialization::IJavaXMLSerialize
        {
        public:
            static const double  c_minTime;
            static const double  c_minProcessingCost;

            static const quint32 c_defaultID;
            static const double  c_defaultExecStartTime;
            static const double  c_defaultExecFinishTime;
            static const double  c_defaultProcessingCost;

        public:
            CGridSimGridletOutput(quint32 id = c_defaultID, double execStartTime = c_defaultExecStartTime,
                double execFinishTime = c_defaultExecFinishTime, double processingCost = c_defaultProcessingCost,
                const QString &history = QString());

            bool operator == (const CGridSimGridletOutput& other) const;
            bool operator != (const CGridSimGridletOutput& other) const;

            quint32 getID() const;
            void setID(quint32 id);

            double getExecStartTime() const;
            void setExecStartTime(double execStartTime);

            double getExecFinishTime() const;
            void setExecFinishTime(double execFinishTime);

            double getProcessingCost() const;
            void setProcessingCost(double processingCost);

            QString getHistory() const;
            void setHistory(const QString &history);

            virtual bool isValid() const;

        protected:
            // You must overload this stubs for data (de)serialization.
            //
            virtual void saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const;
            virtual void loadDataFromXMLNode(const QDomElement &elem);

            // Factory method for creating instances of derived objects
            virtual Core::Serialization::IJavaXMLSerializePtr create() const;

            // Method from copying data from other instance of derived object
            virtual void copy(const Core::Serialization::IJavaXMLSerializePtr &from);

        private:
            quint32 m_id;
            double m_execStartTime;
            double m_execFinishTime;
            double m_processingCost;
            QString m_history;

        private:
            static const QString c_className;
            static const QString c_idPropName;
            static const QString c_execStartTimePropName;
            static const QString c_execFinishTimePropName;
            static const QString c_processingCostPropName;
            static const QString c_HistoryPropName;
        };

        typedef QVector<CGridSimGridletOutput> CGridSimGridletsOutput;
    }
}

Q_DECLARE_METATYPE(Core::GridSimConfig::CGridSimGridletOutput);
