
function func() {
    cppButton.setStyleSheet('QPushButton { background-color: qlineargradient(\
                                                x0:0, y0:0, x1:1, y1:1, \
                                                stop: 0.0 #111111,\
                                                stop: 0.2 #222222,\
                                                stop: 0.4 #444444,\
                                                stop: 0.6 #888888,\
                                                stop: 0.8 #aaaaaa,\
                                                stop: 1.0 #ffffff);\
                                           color:white;}\
                            QPushButton:hover { border:2px solid blue;\
                                                padding:1ex; }\
                            QPushButton:pressed { background-color: qlineargradient(\
                                                x0:0, y0:0, x1:1, y1:1, \
                                                stop: 0.0 #ff1111,\
                                                stop: 0.2 #22ff22,\
                                                stop: 0.4 #4444ff,\
                                                stop: 0.6 #88ee88,\
                                                stop: 0.8 #aaeeaa,\
                                                stop: 1.0 #ffffff); }')
    cppButton.text = 'Changed in JS'
}
func()
