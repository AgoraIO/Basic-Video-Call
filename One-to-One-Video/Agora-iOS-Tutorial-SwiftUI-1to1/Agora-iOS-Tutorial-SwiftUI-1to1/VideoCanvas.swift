//
//  VideoView.swift
//  Agora-iOS-Tutorial-SwiftUI-1to1
//
//  Created by GongYuhua on 2019/6/17.
//  Copyright © 2019 Agora. All rights reserved.
//

import SwiftUI

struct VideoCanvas : UIViewRepresentable {
    let rendererView = UIView()
    
    func makeUIView(context: Context) -> UIView {
        rendererView.backgroundColor = UIColor.gray
        return rendererView
    }
    
    func updateUIView(_ uiView: UIView, context: Context) { }
}

struct VideoView_Previews : PreviewProvider {
    static var previews: some View {
        VideoCanvas()
    }
}
