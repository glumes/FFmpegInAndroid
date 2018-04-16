package com.glumes.base.binder

import android.databinding.DataBindingUtil
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.glumes.databindingadapter.BindingViewHolder
import com.glumes.databindingadapter.ViewHolderBinder
import com.glumes.ffmpeginandroid.R
import com.glumes.ffmpeginandroid.databinding.AdapterBasicTypeBinding

/**
 * Created by glumes on 08/04/2018
 */

data class BasicItem(var title: String, var type: Int)


class BasicItemEventHandler {
    fun onClick(v: View, model: BasicItem) {
        
    }
}


class BasicItemHolder(binding: AdapterBasicTypeBinding) : BindingViewHolder<BasicItem, AdapterBasicTypeBinding>(binding) {

    override fun onBind(data: BasicItem?, position: Int) {
        mBinding.model = data!!
        mBinding.executePendingBindings()
        mBinding.eventHandler = BasicItemEventHandler()
    }

}


class BasicItemBinder : ViewHolderBinder<BasicItem, BasicItemHolder>() {

    override fun onBindViewHolder(holder: BasicItemHolder?, data: BasicItem?, position: Int) {
        holder!!.onBind(data, position)
    }

    override fun createViewHolder(inflater: LayoutInflater, parent: ViewGroup): BasicItemHolder {
        val mBinding = DataBindingUtil.inflate<AdapterBasicTypeBinding>(inflater, R.layout.adapter_basic_type, parent, false)
        return BasicItemHolder(mBinding)
    }
}