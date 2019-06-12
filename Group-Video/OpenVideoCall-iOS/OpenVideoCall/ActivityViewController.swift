//
//  ActivityViewController.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/5/23.
//  Copyright © 2019 Agora. All rights reserved.
//

import UIKit

class ActivityViewController: UIViewController {

    @IBOutlet weak var bgView: UIView!
    
    lazy var activityView : CSActivityView = {
        let view = CSActivityView(size: 70, superview: self.bgView)
        view.dotColor = UIColor.AGGray
        return view
    }()
    
    var isAnimating: Bool = false {
        didSet {
            if isAnimating {
                activityView.startAnimation()
            } else {
                activityView.stopAnimation()
            }
            self.view.isHidden = !isAnimating
        }
    }
}
