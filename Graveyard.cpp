//data_collector image_collector;
//image_collector.save_n_video_frames(100,"../Data/Video_Data/Crowded_Sea_Floor.mp4","../Data/Crowded_Sea_Floor_Images/",0,0);
//data_collector video_maker;
//video_maker.create_video_from_folder("../Data/Crowded_Sea_Floor_Annotated/","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");

//test_methods tests;
// Run first sift contrast tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    for(int i = 0; i < 11; i++){
//        tests.run_test(i);
//    }
//}

// Run first sift contrast tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    for(int i = 0; i < 11; i++){
//        tests.run_test(i);
//    }
//}

// Run first sift contrast tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    for(int i = 0; i < 11; i++){
//        tests.run_test(i);
//    }
//}

// Test if matching works
//string video_path = "../Data/Video_Data/Solo_Pillar.mkv";
//tests.change_video_path(video_path);
//tests.run_test(11);

//vector<string> video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/New_Pillar_Top.mkv","../Data/Video_Data/New_Pillar_Bottom.mkv"};
//vector<string> video_paths = {"../Data/Video_Data/New_Pillar_Top.mkv"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.run_test(12);
//}

//vector<string> video_paths = {"../Data/Video_Data/Tank_With_Pipe.mkv"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.run_test(12);
//}

// Run first sift edge tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(13);
//}

// Run first sift edge tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(13);
//}

// Run first sift edge tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(13);
//}

//sift_settings desired_settings = {1000,3,0.02,10,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/frame_normal","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//sift_settings desired_settings = {1000,3,0.02,5,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/edge_low_frame_normal_crowded","../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");
//desired_settings = {1000,3,0.02,30,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/edge_medium_frame_normal_crowded","../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");
//desired_settings = {1000,3,0.02,60,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/edge_high_frame_normal_crowded","../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");

// Run first sift layer tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(15);
//}

// Run first sift layer tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(15);
//}

// Run first sift layer tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(15);
//}

//sift_settings desired_settings = {1000,5,0.02,43,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/layers_frame_normal_crowded_2","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/layers_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/layers_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first sift sigma tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(16);
//}

// Run first sift sigma tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(16);
//}

// Run first sift sigma tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(16);
//}
//sift_settings desired_settings = {1000,3,0.02,43,1.6,0,false};
//tests.get_feature_video(desired_settings, "../Data/feature_images/sigma_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/sigma_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video(desired_settings, "../Data/feature_images/sigma_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first sift precise upscale tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(17);
//}

// Run first sift precise upscale tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(17);
//}

// Run first sift precise upscale tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(17);
//}

// Run first orb scale factor tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(18);
//}

// Run first orb scale factor tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(18);
//}

// Run first orb scale tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(18);
//}

//orb_settings desired_settings = {1000,1.2,8,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//orb_settings desired_settings = {1000,1.6,8,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/scale_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb levels tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(19);
//}

// Run first orb levels tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//  tests.run_test(19);
//}

// Run first orb levels on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(19);
//}

//orb_settings desired_settings = {1000,1.6,6,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//orb_settings desired_settings = {1000,1.6,16,31,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/layers_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb edge threshold tests on clear videos
//vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(20);
//}

// Run first orb edge threshold tests on tank videos
//vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(20);
//}

// Run first orb edge threshold tests on crowded videos
//vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//for(int j = 0; j < video_paths.size();j++){
//    tests.change_video_path(video_paths.at(j));
//    tests.change_annotation_path(annotation_paths.at(j));
//    tests.run_test(20);
//}

//orb_settings desired_settings = {1000,1.6,6,0,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//desired_settings = {1000,1.6,16,80,0,2,31,20};
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/edge_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb patch size tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(21);
//    }

// Run first orb patch size tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(21);
//    }

// Run first orb patch size tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(21);
//    }


//    orb_settings desired_settings = {1000,1.6,6,2,0,2,2,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,80,0,2,80,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/patch_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

// Run first orb start level tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(22);
//    }

// Run first orb start level tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(22);
//    }

// Run first orb start level tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(22);
//    }

//    orb_settings desired_settings = {1000,1.6,6,34,0,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,3,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_medium_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_medium_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_medium_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,6,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/start_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");


// Run first orb point in element tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(23);
//    }

// Run first orb points in element tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(23);
//    }

// Run first orb points in element tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(23);
//    }

//    orb_settings desired_settings = {1000,1.6,6,34,0,2,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,0,4,34,20};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/points_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");


// Run first orb FAST tests on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(24);
//    }

// Run first orb FAST tests on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(24);
//    }

// Run first orb FAST tests on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(24);
//    }

//    orb_settings desired_settings = {1000,1.6,6,34,0,2,34,10};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_low_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_low_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_low_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,0,2,34,32};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_medium_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_medium_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_medium_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");

//    desired_settings = {1000,1.6,16,34,0,2,34,50};
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_high_frame_normal_crowded","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_high_frame_normal_tank","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi");
//    tests.get_feature_video_orb(desired_settings, "../Data/Orb/Images/fast_high_frame_normal_clear","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi");


// Run comparison on clear videos
//    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(25);
//    }

// Run comparison on tank videos
//    vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(25);
//    }

// Run comparison on crowded videos
//    vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
//    vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.change_annotation_path(annotation_paths.at(j));
//        tests.run_test(25);
//    }


//    vector<string> video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/New_Pillar_Top.mkv","../Data/Video_Data/New_Pillar_Bottom.mkv"};
//    for(int j = 0; j < video_paths.size();j++){
//        tests.change_video_path(video_paths.at(j));
//        tests.run_test(26);
//    }


//Mat grayscaled_last_frame, grayscaled_frame;
//last_frame.frame = cvtColor(last_frame.frame, grayscaled_last_frame, CV_BGR2GRAY);
//frame.frame = cvtColor(frame.frame, grayscaled_frame, CV_BGR2GRAY);

//optical_flow_results old_data[buffer_size];



//        vector<vector<vector<keypoint_data>>> possible_clusters_front;
//        vector<vector<vector<keypoint_data>>> possible_clusters_back;
//        int temp_cluster_count = cluster_count;
//        for(int clusters_remaining = cluster_count; clusters_remaining > 1; clusters_remaining--){
//            cout << "Clusters remaining: " << clusters_remaining << endl;
//            // Find possible outer clusters
//            vector<vector<keypoint_data>> current_front_ranges;
//            vector<vector<keypoint_data>> current_back_ranges;
//            cout << "Creating possibility" << endl;
//            for(int i = 0; i <= sorted_keypoints.size()-temp_cluster_count; i++){
//                int index = sorted_keypoints.size()-temp_cluster_count-i+1;
//                vector<keypoint_data> outer_front;
//                copy(sorted_keypoints.begin(), sorted_keypoints.begin()+index, back_inserter(outer_front));
//                vector<keypoint_data> outer_back;
//                copy(sorted_keypoints.end() - index, sorted_keypoints.end(), back_inserter(outer_back));
//                current_front_ranges.push_back(outer_front);
//                current_back_ranges.push_back(outer_back);
//            }
//            cout << "All posibilities created" << endl;
//            possible_clusters_front.push_back(current_front_ranges);
//            possible_clusters_back.push_back(current_back_ranges);

//            // remove outer values since they are not needed
//            sorted_keypoints.erase(sorted_keypoints.begin());
//            sorted_keypoints.erase(sorted_keypoints.end());

//            clusters_remaining--;
//            temp_cluster_count -= 2;
//        }

//        cout << "Outer clusters found" << endl;
//        cout << sorted_keypoints.size() << " remaining datapoints" << endl;

//        // Add middle range if number of clusters are unewen
//        vector<vector<keypoint_data>> middle_ranges;
//        if(cluster_count % 2 != 0){
//            for(int i = 0; i <= sorted_keypoints.size()-cluster_count; i++){
//                int index = sorted_keypoints.size()-cluster_count-i+1;

//                vector<keypoint_data> front;
//                copy(sorted_keypoints.begin(), sorted_keypoints.begin()+index, back_inserter(front));
//                vector<keypoint_data> back;
//                copy(sorted_keypoints.end() - index, sorted_keypoints.end(), back_inserter(back));

//                middle_ranges.push_back(front);
//                middle_ranges.push_back(back);
//            }
//        }
//        cout << "Middle clusters found" << endl;

//        // Reset sorted_keypoints
//        sorted_keypoints = sorted_keypoints_stored;

//        // Test print of data
//        for(int i = 0; i < possible_clusters_front.size(); i++){
//            cout << "Cluster " << i << " possibilities" << endl;
//            for(int j = 0; j < possible_clusters_front[i].size(); j++){
//                cout << "[";
//                for(int k = 0; k < possible_clusters_front[i].at(j).size(); k++){
//                    cout << possible_clusters_front[i].at(j).at(k).velocity << " ";
//                }
//                cout << "]" << endl;
//            }
//        }

//        // Test print of data
//        for(int i = 0; i < possible_clusters_back.size(); i++){
//            cout << "Cluster " << (cluster_count-i) << " possibilities" << endl;
//            for(int j = 0; j < possible_clusters_back[i].size(); j++){
//                cout << "[";
//                for(int k = 0; k < possible_clusters_back[i].at(j).size(); k++){
//                    cout << possible_clusters_back[i].at(j).at(k).velocity << " ";
//                }
//                cout << "]" << endl;
//            }
//        }

//        // Test print of data
//        cout << "Cluster " << (possible_clusters_front.size()+1) << " possibilities" << endl;
//        for(int j = 0; j < middle_ranges.size(); j++){
//            cout << "[";
//            for(int k = 0; k < middle_ranges[j].size(); k++){
//                cout << middle_ranges[j].at(k).velocity << " ";
//            }
//            cout << "]" << endl;
//        }
