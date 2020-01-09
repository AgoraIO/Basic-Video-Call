//
//  VideoSessionView.swift
//  Agora-iOS-Tutorial-SwiftUI-1to1
//
//  Created by GongYuhua on 2019/6/17.
//  Copyright © 2019 Agora. All rights reserved.
//

import SwiftUI

struct VideoSessionView : View {
    var backColor: Color?
    var backImage: Image?
    var hideCanvas: Bool = false
    var canvas: VideoCanvas?
    
    var body: some View {
        ZStack {
            if hideCanvas {
                backColor
                backImage
            } else {
                canvas
            }
        }
    }
}

struct VideoSessionView_Previews : PreviewProvider {
    static var previews: some View {
        Group {
            VideoSessionView(
                backColor: Color("remoteBackColor"),
                backImage: Image("big_logo"),
                hideCanvas: true
            )
            VideoSessionView(
                backColor: Color("localBackColor"),
                backImage: Image("logo"),
                hideCanvas: true
            )
            VideoSessionView(
                hideCanvas: false
            )
        }
    }
}
