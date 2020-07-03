#pragma once

class QObject; // Forward definition

namespace Engine {
    namespace Gui {
        /*!
         * @brief Some generic gui widget class
         * @ingroup Gui
         */
        class GuiWidget {
        public:
            explicit GuiWidget(QObject* parent);
            virtual ~GuiWidget() = default;

        signals:
            void mySignal(QObject* object, int a, bool b);

        public slots:
            void mySlot(QObject* object, int a, bool b);

        private:
            QObject* parent;
        };
    } // namespace Gui
} // namespace Engine
